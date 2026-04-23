#include "slab_allocator.h"
#include <stdio.h>

int main(){
  printf("demo alloc starting\n");
  if(gpu_allocator_init(2*1024*1024) != 0){
    fprintf(stderr, "allocator init failed\n");
    return 1;
  }
  void* p[10];
  for(int i=0;i<10;i++){
    gpu_alloc_handle_t h = gpu_alloc(1024*(i+1), 0);
    p[i] = h.ptr;
    printf("alloc %d -> %p\n", i, h.ptr);
  }
  for(int i=0;i<10;i+=2){
    gpu_free((gpu_alloc_handle_t){p[i], 0, 0});
    printf("freed %p\n", p[i]);
  }
  gpu_allocator_shutdown();
  printf("demo alloc done\n");
  return 0;
}
