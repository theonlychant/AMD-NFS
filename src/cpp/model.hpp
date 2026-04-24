// model.hpp
#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <memory>

struct ModelConfig {
    int32_t n_layers   = 32;
    int32_t n_heads    = 32;
    int32_t n_kv_heads = 32;
    int32_t dim        = 4096;
    int32_t ffn_hidden = 11008;
    int32_t vocab_size = 32000;
    int32_t max_seq    = 2048;
};

class Model {
public:
    explicit Model(const ModelConfig& cfg);
    ~Model();

    bool load_weights(const std::string& path);

    // Single forward step; returns logits over vocab
    // token_ids: current context, step: position index
    std::vector<float> forward(const std::vector<int32_t>& token_ids,
                               int32_t step);

    const ModelConfig& config() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};