# cython: language_level=3
from libcpp.string cimport string
cdef extern from "../src/cpp/engine.hpp" namespace "":
    cdef cppclass InferenceEngine:
        InferenceEngine()
        bool load_model(const string &path)
        string generate(const string &prompt, int max_tokens=32)

cdef class PyInferenceSession:
    cdef InferenceEngine* engine
    def __cinit__(self):
        self.engine = new InferenceEngine()
    def __dealloc__(self):
        del self.engine

    def load_model(self, path: str):
        cdef string p = path.encode('utf-8')
        return self.engine.load_model(p)

    def generate(self, prompt: str, max_tokens: int=32):
        cdef string p = prompt.encode('utf-8')
        cdef string out = self.engine.generate(p, max_tokens)
        return out.decode('utf-8')
