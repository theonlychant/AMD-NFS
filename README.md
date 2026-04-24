# AMD-Native Inference Stack (scaffold)

This repository is a scaffold for an AMD-native LLM inference stack targeting ROCm/HIP.

Structure:
- `src/c` - C layer: slab allocator, HIP kernel stubs
- `src/cpp` - C++ engine core skeleton
- `py` - Cython bindings and setup.py
- `cmd/server` - Go server skeleton

Quick build (requires cmake, python, go):

```bash
cmake -S . -B build -DUSE_HIP=OFF
cmake --build build -j4
cd py && python -m pip install -e .
cd cmd/server && go build -v ./...
```

Notes:
- This is an initial scaffold. Implementations are prototypes and stubs.
- For HIP/ROCm builds, ensure `hipcc` is in PATH and set `-DUSE_HIP=ON`.