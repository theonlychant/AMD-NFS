// Free-list slab allocator prototype using a single arena (pinned host) + HIP fallback
#include "slab_allocator.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __has_include
# if __has_include(<hip/hip_runtime.h>)
#  include <hip/hip_runtime.h>
#  define HAVE_HIP 1
# endif
#endif

// Block header stored in the arena for free-list management
typedef struct free_block {
  size_t size; // size of this block, including header
  struct free_block* next;
} free_block_t;

static void* arena_base = NULL;
static size_t arena_size = 0;
static free_block_t* free_list = NULL;

// Alignment for allocations
#define ALIGNMENT 64
#define ALIGN_UP(n, a) (((n) + (a) - 1) & ~((a) - 1))
#define HEADER_SIZE ALIGN_UP(sizeof(free_block_t), ALIGNMENT)

int gpu_allocator_init(size_t total_bytes){
  if(total_bytes == 0) return -1;
  arena_size = ALIGN_UP(total_bytes, ALIGNMENT);
#ifdef HAVE_HIP
  if(hipHostMalloc(&arena_base, arena_size, hipHostMallocDefault) != hipSuccess){
    arena_base = NULL;
  }
#endif
  if(arena_base == NULL){
    arena_base = malloc(arena_size);
    if(!arena_base) return -1;
  }
  // initialize a single free block covering the whole arena
  free_list = (free_block_t*)arena_base;
  free_list->size = arena_size;
  free_list->next = NULL;
  return 0;
}

void gpu_allocator_shutdown(){
  // free arena
#ifdef HAVE_HIP
  if(arena_base) hipHostFree(arena_base);
  arena_base = NULL;
#else
  free(arena_base);
  arena_base = NULL;
#endif
  arena_size = 0;
  free_list = NULL;
}

static void insert_and_coalesce(free_block_t* block){
  // Insert block into free_list sorted by address, then coalesce neighbors
  if(!free_list){
    free_list = block;
    block->next = NULL;
    return;
  }
  free_block_t* prev = NULL;
  free_block_t* cur = free_list;
  while(cur && cur < block){
    prev = cur;
    cur = cur->next;
  }
  // insert
  block->next = cur;
  if(prev) prev->next = block; else free_list = block;

  // coalesce with next
  if(block->next && (char*)block + block->size == (char*)block->next){
    block->size += block->next->size;
    block->next = block->next->next;
  }
  // coalesce with prev
  if(prev && (char*)prev + prev->size == (char*)block){
    prev->size += block->size;
    prev->next = block->next;
  }
}

gpu_alloc_handle_t gpu_alloc(size_t bytes, int device_mem){
  gpu_alloc_handle_t h = {0};
  h.size = bytes;
  h.on_device = device_mem;
  if(device_mem){
#ifdef HAVE_HIP
    if(hipMalloc(&h.ptr, bytes) != hipSuccess){
      h.ptr = NULL;
    }
    return h;
#else
    // no HIP - fail allocation
    h.ptr = NULL;
    return h;
#endif
  }

  // arena-backed allocation (simple first-fit)
  size_t req = ALIGN_UP(bytes, ALIGNMENT) + HEADER_SIZE;
  free_block_t* prev = NULL;
  free_block_t* cur = free_list;
  while(cur){
    if(cur->size >= req){
      // found a block
      if(cur->size - req >= (ssize_t)(HEADER_SIZE + ALIGNMENT)){
        // split
        free_block_t* newblock = (free_block_t*)((char*)cur + req);
        newblock->size = cur->size - req;
        newblock->next = cur->next;
        cur->size = req;
        if(prev) prev->next = newblock; else free_list = newblock;
      } else {
        // allocate whole block
        if(prev) prev->next = cur->next; else free_list = cur->next;
      }
      // user pointer is after header
      h.ptr = (void*)((char*)cur + HEADER_SIZE);
      return h;
    }
    prev = cur;
    cur = cur->next;
  }
  // out of memory
  h.ptr = NULL;
  return h;
}

void gpu_free(gpu_alloc_handle_t h){
  if(!h.ptr) return;
  if(h.on_device){
#ifdef HAVE_HIP
    hipFree(h.ptr);
#endif
    return;
  }
  // compute header address from user pointer and insert into free list
  free_block_t* block = (free_block_t*)((char*)h.ptr - HEADER_SIZE);
  // Ensure size is sane; otherwise ignore
  if(block->size == 0) return;
  insert_and_coalesce(block);
}

