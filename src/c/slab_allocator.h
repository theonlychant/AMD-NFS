// Thin slab allocator for GPU VRAM (free-list + coalescing prototype)
#pragma once
#include <stddef.h>

typedef struct gpu_alloc_handle {
  void* ptr;
  size_t size;
  int on_device; // 1 if device memory, 0 if pinned host (managed by free-list)
} gpu_alloc_handle_t;

// Initialize the allocator with a backing arena (pinned host memory for zero-copy)
// total_bytes: size of arena in bytes. Returns 0 on success
int gpu_allocator_init(size_t total_bytes);
void gpu_allocator_shutdown();

// Allocate bytes from the arena (device_mem==0 uses arena/pinned host),
// device_mem==1 falls back to hipMalloc when HIP is available.
gpu_alloc_handle_t gpu_alloc(size_t bytes, int device_mem);
void gpu_free(gpu_alloc_handle_t h);

