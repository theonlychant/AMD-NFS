# cython: language_level=3
from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp cimport bool as cppbool
cimport cython

cdef extern from "../src/cpp/engine.hpp" namespace "":
    cdef cppclass InferenceEngine:
        InferenceEngine()
        cppbool load_model(const string& path)
        string generate(const string& prompt, int max_tokens)

cdef extern from "../src/cpp/tokenizer.hpp" namespace "":
    cdef cppclass Tokenizer:
        Tokenizer(const string& vocab_path)
        vector[int] encode(const string& text) except +
        string decode(const vector[int]& ids) except +
        int vocab_size()
        int bos_id()
        int eos_id()


cdef class PyTokenizer:
    cdef Tokenizer* tok
    def __cinit__(self, vocab_path: str=""):
        cdef string p = string(vocab_path.encode('utf-8'))
        self.tok = new Tokenizer(p)
    def __dealloc__(self):
        del self.tok

    def encode(self, text: str):
        cdef string t = string(text.encode('utf-8'))
        cdef vector[int] v = self.tok.encode(t)
        pylist = [v[i] for i in range(v.size())]
        return pylist

    def decode(self, ids):
        # accept list of ints
        cdef vector[int] v
        for i in ids:
            v.push_back(<int>i)
        cdef string s = self.tok.decode(v)
        cdef bytes b = s.c_str()
        return b.decode('utf-8')


cdef class PyInferenceSession:
    cdef InferenceEngine* engine
    def __cinit__(self):
        self.engine = new InferenceEngine()
    def __dealloc__(self):
        del self.engine

    def load_model(self, path: str):
        cdef string p = string(path.encode('utf-8'))
        cdef cppbool ok = self.engine.load_model(p)
        return True if ok else False

    def generate(self, prompt: str, max_tokens: int=32):
        cdef string p = string(prompt.encode('utf-8'))
        cdef string out = self.engine.generate(p, max_tokens)
        cdef bytes b = out.c_str()
        return b.decode('utf-8')
