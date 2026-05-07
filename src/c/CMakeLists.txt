cmake_minimum_required(VERSION 3.18)
add_library(amdinfer_c STATIC
  slab_allocator.c
  hip_kernels.c
)

target_include_directories(amdinfer_c PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Attempt to detect rocBLAS (ROCm) and system BLAS for faster GEMM
set(EXTRA_LIBS "")
set(EXTRA_INCLUDES "")

if(USE_HIP AND HIPCC)
  # Attempt to compile with hipcc where available
  set_target_properties(amdinfer_c PROPERTIES
    COMPILE_FLAGS "-x hip"
  )
endif()

# Look for rocBLAS in /opt/rocm by default
find_path(ROCBLAS_INCLUDE rocblas.h HINTS /opt/rocm/include)
find_library(ROCBLAS_LIB NAMES rocblas HINTS /opt/rocm/lib)
if(ROCBLAS_LIB AND ROCBLAS_INCLUDE)
  message(STATUS "Found rocBLAS: ${ROCBLAS_LIB}")
  list(APPEND EXTRA_LIBS ${ROCBLAS_LIB})
  list(APPEND EXTRA_INCLUDES ${ROCBLAS_INCLUDE})
  target_compile_definitions(amdinfer_c PUBLIC USE_ROCBLAS=1)
endif()

# Try to find system BLAS (OpenBLAS, Intel MKL, etc.) as a CPU fallback
find_package(BLAS QUIET)
if(BLAS_FOUND)
  message(STATUS "Found BLAS libraries: ${BLAS_LIBRARIES}")
  list(APPEND EXTRA_LIBS ${BLAS_LIBRARIES})
  target_compile_definitions(amdinfer_c PUBLIC HAVE_CBLAS=1)
endif()

if(EXTRA_INCLUDES)
  target_include_directories(amdinfer_c PUBLIC ${EXTRA_INCLUDES})
endif()

if(EXTRA_LIBS)
  target_link_libraries(amdinfer_c PRIVATE ${EXTRA_LIBS})
endif()

add_executable(test_slab test_slab.c)
target_link_libraries(test_slab PRIVATE amdinfer_c)

add_executable(demo_alloc demo_alloc.c)
target_link_libraries(demo_alloc PRIVATE amdinfer_c)
