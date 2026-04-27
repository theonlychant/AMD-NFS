Python ML helper package for AMD-Native Inference (py/)

Quick start (without heavy ML deps):

1. Install editable package (uses compiled extension if available):

```bash
pip install -e .
```

2. Run the simple CLI:

```bash
amdinfer-cli "Hello world"
```

Optional ML extras (CPU/GPU libs like torch):

```bash
pip install -e .[ml]
```

The package will prefer the compiled `amdinfer_py` extension if available; otherwise
it falls back to a pure-Python stub implementation suitable for tests and CI.
