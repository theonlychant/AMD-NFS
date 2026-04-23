cmake_minimum_required(VERSION 3.18)
add_library(amdinfer_c STATIC
  slab_allocator.c
  hip_kernels.c
)

target_include_directories(amdinfer_c PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(USE_HIP AND HIPCC)
  # Attempt to compile with hipcc where available
  set_target_properties(amdinfer_c PROPERTIES
    COMPILE_FLAGS "-x hip"
  )
endif()
