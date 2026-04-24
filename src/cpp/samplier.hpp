// sampler.hpp
#pragma once
#include <vector>
#include <cstdint>
#include <cstddef>

struct SamplerConfig {
    float    temperature = 1.0f;
    float    top_p       = 0.9f;
    uint64_t seed        = 42;
};

class Sampler {
public:
    explicit Sampler(const SamplerConfig& cfg = {});

    int32_t sample(const std::vector<float>& logits);
    int32_t argmax(const std::vector<float>& logits) const;

private:
    SamplerConfig cfg_;
    uint64_t      rng_state_;
};