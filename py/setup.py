from setuptools import setup, Extension
from Cython.Build import cythonize
import sys

import glob

sources = ["inference.pyx"]
# include C/C++ source files so the extension links everything together
# exclude test/demo files which define `main`
c_files = [f for f in glob.glob("../src/c/*.c") if not ("test" in f or "demo" in f)]
cpp_files = [f for f in glob.glob("../src/cpp/*.cpp") if not ("test" in f or "demo" in f)]
sources += c_files + cpp_files

ext = Extension(
    "amdinfer_py._amdinfer",
    sources,
    language="c++",
    include_dirs=["../src/c", "../src/cpp"],
    extra_compile_args=["-std=c++20"],
    extra_link_args=[],
)

setup(
    name="amdinfer_py",
    packages=["amdinfer_py", "amdinfer_ml"],
    ext_modules=cythonize([ext], compiler_directives={"language_level": "3"}),
    entry_points={
        "console_scripts": [
            "amdinfer-cli=amdinfer_ml.cli:main",
        ]
    },
    extras_require={
        "ml": ["torch", "tokenizers", "numpy"],
    },
)
# Author: theonlychant