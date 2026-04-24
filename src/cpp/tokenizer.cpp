// tokenizer.cpp
#include "tokenizer.hpp"
#include <stdexcept>

struct Tokenizer::Impl {
    // BPE / sentencepiece handle would live here(to be honest I probably don't need it)
    int32_t vocab_sz = 32000;
    int32_t bos = 1, eos = 2;
};

Tokenizer::Tokenizer(const std::string& vocab_path)
    : impl(new Impl())
{
    // TODO: load vocab from vocab_path
    (void)vocab_path;
}
Tokenizer::~Tokenizer() = default;

std::vector<int32_t> Tokenizer::encode(const std::string& text) const {
    // stub: one token per char
    std::vector<int32_t> ids;
    ids.reserve(text.size() + 2);
    ids.push_back(impl->bos);
    for (char c : text) ids.push_back(static_cast<uint8_t>(c));
    return ids;
}
std::string Tokenizer::decode(const std::vector<int32_t>& ids) const {
    std::string out;
    for (int32_t id : ids)
        if (id != impl->bos && id != impl->eos)
            out += static_cast<char>(id);
    return out;
}
int32_t Tokenizer::vocab_size() const { return impl->vocab_sz; }
int32_t Tokenizer::bos_id()    const { return impl->bos; }
int32_t Tokenizer::eos_id()    const { return impl->eos; }