"""amdinfer_py package wrapper.

This module exposes Python-friendly names imported from the compiled
extension `._amdinfer`. Import tests can use:

  from amdinfer_py import PyInferenceSession, PyTokenizer

"""
try:
    from ._amdinfer import PyInferenceSession, PyTokenizer
except Exception:
    # fallback: try top-level import (if extension was built differently)
    try:
        from _amdinfer import PyInferenceSession, PyTokenizer  # type: ignore
    except Exception:
        # make imports fail with clear error
        raise ImportError("amdinfer_py compiled extension not found. Build the extension first.")

__all__ = ["PyInferenceSession", "PyTokenizer"]
