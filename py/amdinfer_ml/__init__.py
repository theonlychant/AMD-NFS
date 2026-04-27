"""High-level Python ML interface for AMD-Native Inference.

This module provides a pure-Python fallback implementation and a thin
integration layer to the compiled extension `amdinfer_py` when available.

APIs:
- `MLInferenceSession` : high-level session with `load_model()` and `generate()`.
- `get_tokenizer()` : returns a tokenizer object with `encode`/`decode`.
"""
from importlib import import_module
try:
    # compiled extension wrapper we built earlier
    from amdinfer_py import PyInferenceSession, PyTokenizer
    _has_native = True
except Exception:
    PyInferenceSession = None
    PyTokenizer = None
    _has_native = False

from .session import MLInferenceSession, get_tokenizer

__all__ = ["MLInferenceSession", "get_tokenizer", "_has_native"]
