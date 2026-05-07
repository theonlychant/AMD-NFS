#include "engine.hpp"
#include <iostream>
#include "booster.hpp"

struct InferenceEngine::Impl {
  // Minimal implementation details for the stub engine
  Impl(){ booster::init(); }
};

InferenceEngine::InferenceEngine(): impl(new Impl()){}
InferenceEngine::~InferenceEngine() = default;

bool InferenceEngine::load_model(const std::string &path){
  std::cerr<<"[engine] load_model: "<<path<<" (stub)\n";
  return true;
}

std::string InferenceEngine::generate(const std::string &prompt, int max_tokens){
  (void)max_tokens;
  // Use booster when available to form a boosted response
  std::string boosted = booster::generate_boosted(prompt);
  return boosted;
}
