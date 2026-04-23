// Thin slab allocator for GPU VRAM (prototype)
#pragma once
#include <stddef.h>

typedef struct gpu_alloc_handle {
  void* ptr;
  size_t size;
  int on_device; // 1 if device memory, 0 if pinned host
} gpu_alloc_handle_t;

int gpu_allocator_init(size_t total_bytes);
void gpu_allocator_shutdown();
gpu_alloc_handle_t gpu_alloc(size_t bytes, int device_mem);
void gpu_free(gpu_alloc_handle_t h);
