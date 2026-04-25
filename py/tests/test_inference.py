import pytest

def test_session_basic():
    from amdinfer_py import PyInferenceSession, PyTokenizer
    s = PyInferenceSession()
    assert s.load_model('stub') is True
    out = s.generate('hello', max_tokens=4)
    assert isinstance(out, str)

def test_tokenizer_roundtrip():
    from amdinfer_py import PyTokenizer
    t = PyTokenizer("")
    ids = t.encode("abc")
    assert isinstance(ids, list)
    out = t.decode(ids)
    assert isinstance(out, str)
