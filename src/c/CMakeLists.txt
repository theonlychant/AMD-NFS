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

add_executable(test_slab test_slab.c)
target_link_libraries(test_slab PRIVATE amdinfer_c)

add_executable(demo_alloc demo_alloc.c)
target_link_libraries(demo_alloc PRIVATE amdinfer_c)
