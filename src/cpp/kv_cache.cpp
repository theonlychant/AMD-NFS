// kv_cache.cpp
// Author: theonlychant
#include "kv_cache.hpp"
#include <stdexcept>
#include <cstring>

KVCache::KVCache(int32_t n_layers, int32_t n_kv_heads,
                 int32_t head_dim, int32_t max_seq)
    : n_layers_(n_layers), n_kv_heads_(n_kv_heads),
      head_dim_(head_dim), max_seq_(max_seq),
      cur_len_(0)
{
    const size_t layer_sz = static_cast<size_t>(n_kv_heads) *
                            static_cast<size_t>(max_seq)    *
                            static_cast<size_t>(head_dim);

    k_store_.resize(n_layers, std::vector<float>(layer_sz, 0.f));
    v_store_.resize(n_layers, std::vector<float>(layer_sz, 0.f));
}

KVCache::~KVCache() = default;

void KVCache::clear() {
    cur_len_ = 0;
    for (auto& layer : k_store_) std::fill(layer.begin(), layer.end(), 0.f);
    for (auto& layer : v_store_) std::fill(layer.begin(), layer.end(), 0.f);
}

int32_t KVCache::current_length() const {
    return cur_len_;
}

float* KVCache::key_ptr(int32_t layer) {
    if (layer < 0 || layer >= n_layers_)
        throw std::out_of_range("KVCache::key_ptr: layer index out of range");
    return k_store_[layer].data();
}

float* KVCache::value_ptr(int32_t layer) {
    if (layer < 0 || layer >= n_layers_)
        throw std::out_of_range("KVCache::value_ptr: layer index out of range");
    return v_store_[layer].data();
}