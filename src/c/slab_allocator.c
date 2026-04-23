// Minimal slab allocator prototype using HIP runtime when available
#include "slab_allocator.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef __has_include
# if __has_include(<hip/hip_runtime.h>)
#  include <hip/hip_runtime.h>
#  define HAVE_HIP 1
# endif
#endif

static size_t g_total = 0;

int gpu_allocator_init(size_t total_bytes){
  g_total = total_bytes;
  // For prototype, nothing else allocated eagerly
  return 0;
}

void gpu_allocator_shutdown(){
  g_total = 0;
}

gpu_alloc_handle_t gpu_alloc(size_t bytes, int device_mem){
  gpu_alloc_handle_t h = {0};
  h.size = bytes;
  h.on_device = device_mem;
#ifdef HAVE_HIP
  hipError_t err;
  if(device_mem){
    err = hipMalloc(&h.ptr, bytes);
    if(err!=hipSuccess) h.ptr = NULL;
  } else {
    err = hipHostMalloc(&h.ptr, bytes, hipHostMallocDefault);
    if(err!=hipSuccess) h.ptr = NULL;
  }
#else
  // Fallback to host malloc for prototype
  h.ptr = malloc(bytes);
#endif
  return h;
}

void gpu_free(gpu_alloc_handle_t h){
#ifdef HAVE_HIP
  if(h.on_device){
    hipFree(h.ptr);
  } else {
    hipHostFree(h.ptr);
  }
#else
  free(h.ptr);
#endif
}
