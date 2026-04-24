// kv_cache.hpp
// author: theonlychant
#pragma once
#include <vector>
#include <cstdint>

// Stores K and V tensors for each layer, growing with each decode step.
// Actual device allocation happens inside; host view is just metadata.
class KVCache {
public:
    KVCache(int32_t n_layers, int32_t n_kv_heads,
            int32_t head_dim, int32_t max_seq);
    ~KVCache();

    void clear();
    int32_t current_length() const;

    // Returns raw host pointers for the stub; real impl hands back HIP ptrs
    float* key_ptr  (int32_t layer);
    float* value_ptr(int32_t layer);

private:
    int32_t n_layers_, n_kv_heads_, head_dim_, max_seq_;
    int32_t cur_len_ = 0;
    std::vector<std::vector<float>> k_store_, v_store_;
};