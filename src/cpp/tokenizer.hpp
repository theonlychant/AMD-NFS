// tokenizer.hpp
#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <memory>

class Tokenizer {
public:
    explicit Tokenizer(const std::string& vocab_path);
    ~Tokenizer();

    std::vector<int32_t> encode(const std::string& text) const;
    std::string          decode(const std::vector<int32_t>& ids) const;
    int32_t              vocab_size() const;
    int32_t              bos_id()    const;
    int32_t              eos_id()    const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};