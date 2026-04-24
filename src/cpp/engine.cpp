#include "engine.hpp"
#include <iostream>

struct InferenceEngine::Impl {
    ModelConfig  cfg;
    Model        model{cfg};
    Tokenizer    tokenizer{""};
    Sampler      sampler{};
    KVCache      kv_cache{cfg.n_layers, cfg.n_kv_heads, cfg.dim / cfg.n_heads, cfg.max_seq};
};
  
  // might add new pointers
  Impl(){}

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
