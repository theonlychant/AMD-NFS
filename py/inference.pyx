# cython: language_level=3
# Still being worked on right now
from libcpp.string cimport string

cdef extern from "../src/cpp/engine.hpp" namespace "":
    cdef cppclass InferenceEngine:
        InferenceEngine()
        bint load_model(const string& path)
        string generate(const string& prompt, int max_tokens)


cdef class PyInferenceSession:
    cdef InferenceEngine* engine
    def __cinit__(self):
        self.engine = new InferenceEngine()
    def __dealloc__(self):
        del self.engine

    def load_model(self, path: str):
        cdef string p = string(path.encode('utf-8'))
        cdef bint ok = self.engine.load_model(p)
        return True if ok else False

    def generate(self, prompt: str, max_tokens: int=32):
        cdef string p = string(prompt.encode('utf-8'))
        cdef string out = self.engine.generate(p, max_tokens)
        cdef const char* s = out.c_str()
        return s
