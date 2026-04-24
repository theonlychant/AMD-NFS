// model.cpp
#include "model.hpp"
#include <iostream>
#include <stdexcept>

struct Model::Impl {
    ModelConfig cfg;
    bool        weights_loaded = false;
    // device buffers (HIP pointers) go here later
};

Model::Model(const ModelConfig& cfg) : impl(new Impl{cfg}) {}
Model::~Model() = default;

bool Model::load_weights(const std::string& path) {
    std::cerr << "[model] load_weights: " << path << " (stub)\n";
    impl->weights_loaded = true;
    return true;
}

std::vector<float> Model::forward(const std::vector<int32_t>& token_ids,
                                  int32_t step)
{
    (void)token_ids; (void)step;
    // stub: uniform logits
    return std::vector<float>(impl->cfg.vocab_size, 0.0f);
}

const ModelConfig& Model::config() const { return impl->cfg; }