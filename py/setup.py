# Setup.py: detect HIP/ROCm and configure extension accordingly
from setuptools import setup, Extension
import sys
import os
import glob
import shutil

# Try to import Cython; if unavailable, fall back to using the
# pre-generated `inference.cpp` so builds don't require Cython.
try:
    from Cython.Build import cythonize
    HAVE_CYTHON = True
except Exception:
    HAVE_CYTHON = False

# Default source: prefer .pyx when Cython available, else use the
# generated .cpp file checked into the repo.
sources = ["inference.pyx"] if HAVE_CYTHON else ["inference.cpp"]
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

# Detect HIP/ROCm environment. Users can force by setting USE_HIP=1
use_hip_env = os.environ.get("USE_HIP")
hipcc = shutil.which("hipcc")
rocm_include = "/opt/rocm/include"
if use_hip_env == "1" or hipcc or os.path.isdir(rocm_include):
    # Add macro so C code can enable HIP paths when headers are available
    ext.define_macros = getattr(ext, 'define_macros', []) + [("HAVE_HIP", "1")]
    ext.extra_compile_args += ["-DHAVE_HIP"]
    # Add ROCm include/lib paths if present
    if os.path.isdir(rocm_include):
        ext.include_dirs.append(rocm_include)
        ext.extra_link_args += ["-L/opt/rocm/lib", "-lamdhip64"]


if HAVE_CYTHON:
    ext_modules = cythonize([ext], compiler_directives={"language_level": "3"})
else:
    # When Cython isn't present, setuptools can still build the extension
    # from the pre-generated C++ file `inference.cpp`.
    ext_modules = [ext]

setup(
    name="amdinfer_py",
    packages=["amdinfer_py", "amdinfer_ml"],
    ext_modules=ext_modules,
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