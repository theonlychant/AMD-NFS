#pragma once
#include <string>
#include <memory>

class InferenceEngine {
public:
  InferenceEngine();
  ~InferenceEngine();

  bool load_model(const std::string &path);
  std::string generate(const std::string &prompt, int max_tokens=32);
private:
  struct Impl;
  std::unique_ptr<Impl> impl;
};
