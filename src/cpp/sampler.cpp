// sampler.cpp
#include "sampler.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdexcept>

Sampler::Sampler(const SamplerConfig& cfg)
    : cfg_(cfg), rng_state_(cfg.seed) {}

int32_t Sampler::argmax(const std::vector<float>& logits) const {
    return static_cast<int32_t>(
        std::max_element(logits.begin(), logits.end()) - logits.begin());
}

int32_t Sampler::sample(const std::vector<float>& logits) {
    if (cfg_.temperature == 0.0f) return argmax(logits);

    // scale
    std::vector<float> scaled(logits.size());
    for (size_t i = 0; i < logits.size(); ++i)
        scaled[i] = logits[i] / cfg_.temperature;

    // softmax
    float mx = *std::max_element(scaled.begin(), scaled.end());
    float sum = 0.f;
    for (auto& v : scaled) { v = std::exp(v - mx); sum += v; }
    for (auto& v : scaled)   v /= sum;

    // simple LCG for now (replace with xoshiro or HIP rng later)
    rng_state_ = rng_state_ * 6364136223846793005ULL + 1442695040888963407ULL;
    float r = static_cast<float>(rng_state_ >> 33) / static_cast<float>(1u << 31);

    float cumsum = 0.f;
    for (int32_t i = 0; i < static_cast<int32_t>(scaled.size()); ++i) {
        cumsum += scaled[i];
        if (r < cumsum) return i;
    }
    return static_cast<int32_t>(scaled.size()) - 1;
}