import sys
import pytest

from amdinfer_ml.session import MLInferenceSession, get_tokenizer


def test_session_and_generate():
    sess = MLInferenceSession()
    assert sess.load_model("/nonexistent/path") is True
    out = sess.generate("hello world", max_tokens=8)
    assert isinstance(out, str)


def test_tokenizer_roundtrip():
    tok = get_tokenizer()
    ids = tok.encode("abc")
    assert isinstance(ids, list)
    s = tok.decode(ids)
    assert isinstance(s, str)


def test_cli_invocation(monkeypatch, capsys):
    import importlib
    import sys as _sys
    monkeypatch.setattr(_sys, "argv", ["amdinfer-cli", "hi"])
    # import the CLI module fresh and call main
    cli = importlib.import_module("amdinfer_ml.cli")
    cli.main()
    captured = capsys.readouterr()
    assert captured.out
