"""ML inference session wrappers.

This module provides a high-level `MLInferenceSession` which will prefer the
compiled C++/Cython backend if available, falling back to a tiny pure-Python
implementation suitable for tests and CPU-only usage.
"""
from typing import List
try:
    from amdinfer_py import PyInferenceSession
    _HAS_NATIVE = True
except Exception:
    PyInferenceSession = None
    _HAS_NATIVE = False

def get_tokenizer(vocab_path: str = ""):
    try:
        from amdinfer_py import PyTokenizer
        return PyTokenizer(vocab_path)
    except Exception:
        # fallback simple tokenizer
        class SimpleTok:
            def __init__(self):
                self.bos = 1
                self.eos = 2
            def encode(self, s: str):
                return [self.bos] + [ord(c) for c in s]
            def decode(self, ids: List[int]):
                return ''.join(chr(i) for i in ids if i not in (self.bos, self.eos))
        return SimpleTok()


class MLInferenceSession:
    def __init__(self):
        if _HAS_NATIVE:
            self._sess = PyInferenceSession()
            self._native = True
        else:
            self._sess = None
            self._native = False

    def load_model(self, path: str) -> bool:
        if self._native:
            return self._sess.load_model(path)
        # fallback: pretend loading
        self._model = {"path": path}
        return True

    def generate(self, prompt: str, max_tokens: int = 32) -> str:
        if self._native:
            return self._sess.generate(prompt, max_tokens=max_tokens)
        # fallback deterministic stub
        return "[stub response] " + prompt
