#include "engine.hpp"
#include <iostream>

struct InferenceEngine::Impl {
  // Minimal implementation details for the stub engine
  Impl(){}
};

InferenceEngine::InferenceEngine(): impl(new Impl()){}
InferenceEngine::~InferenceEngine() = default;

bool InferenceEngine::load_model(const std::string &path){
  std::cerr<<"[engine] load_model: "<<path<<" (stub)\n";
  return true;
}

std::string InferenceEngine::generate(const std::string &prompt, int max_tokens){
  (void)max_tokens;
  return std::string("[stub response] ")+prompt;
}
