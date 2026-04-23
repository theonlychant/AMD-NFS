#include "slab_allocator.h"
#include <stdio.h>

int main(){
  if(gpu_allocator_init(1024*1024) != 0){
    fprintf(stderr, "allocator init failed\n");
    return 1;
  }
  gpu_alloc_handle_t a = gpu_alloc(128, 0);
  gpu_alloc_handle_t b = gpu_alloc(256, 0);
  gpu_alloc_handle_t c = gpu_alloc(64, 0);
  if(!a.ptr || !b.ptr || !c.ptr){
    fprintf(stderr, "alloc failed\n");
    return 2;
  }
  gpu_free(b);
  // allocate something that fits into freed block to test reuse
  gpu_alloc_handle_t d = gpu_alloc(200, 0);
  if(!d.ptr){
    fprintf(stderr, "realloc into freed block failed\n");
    return 3;
  }
  gpu_free(a);
  gpu_free(c);
  gpu_free(d);
  gpu_allocator_shutdown();
  printf("slab allocator test completed\n");
  return 0;
}
