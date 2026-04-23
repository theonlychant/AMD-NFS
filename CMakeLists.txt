cmake_minimum_required(VERSION 3.18)
project(amd_native_inference LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

option(USE_HIP "Build HIP kernels" ON)

if(USE_HIP)
  find_program(HIPCC hipcc)
  if(NOT HIPCC)
    message(WARNING "hipcc not found - HIP sources will be added but may not compile")
  endif()
endif()

add_subdirectory(src/c)
add_subdirectory(src/cpp)
add_subdirectory(py)
add_subdirectory(cmd/server)
