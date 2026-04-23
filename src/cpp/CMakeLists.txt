add_library(amdinfer_cpp STATIC
  engine.cpp
)

target_include_directories(amdinfer_cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(amdinfer_cpp PRIVATE amdinfer_c)
