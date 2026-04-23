from setuptools import setup, Extension
from Cython.Build import cythonize
import sys

ext = Extension(
    "amdinfer_py",
    ["inference.pyx"],
    language="c++",
    include_dirs=["../src/c", "../src/cpp"],
    extra_compile_args=["-std=c++20"],
)

setup(
    name="amdinfer_py",
    ext_modules=cythonize([ext], compiler_directives={"language_level": "3"}),
)
