add_library(amdinfer_cpp STATIC
  engine.cpp
  kv_cache.cpp
  model.cpp
  sampler.cpp
  tokenizer.cpp
)

target_include_directories(amdinfer_cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(amdinfer_cpp PRIVATE amdinfer_c)
