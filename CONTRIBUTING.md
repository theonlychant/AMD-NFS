# Contributing

Thanks for your interest in contributing to AMD-Native Inference. This project is young and opinionated; these guidelines keep contributions reviewable and CI-friendly.

How to contribute
- Fork the repo and open a feature branch named `feat/short-desc` or `fix/short-desc`.
- Keep changes small and focused; one purpose per PR.
- Write a clear PR description describing what you changed and why.

Local dev setup
- Ensure you have: git, cmake, a modern C/C++ toolchain, Python 3.10+, `pip`, and Go 1.20+.
- Build native components:
```bash
cmake -S . -B build -DUSE_HIP=OFF
cmake --build build -j$(nproc)
```
- Build & install the Python extension (dev editable):
```bash
cd py
python -m pip install -e .
```
- Build server and CLI:
```bash
cd cmd/server
go build -o amdinfer_server main.go
go build -o amdinfer_ctl ./cmd/ctl/built.go
```

Testing
- C/C++ unit tests live under `src/c` and are built by CMake. Run the test binaries in `build/`.
- Python tests (pytest) should be added under `py/tests` and run with `pytest` from the `py` folder.

Style & quality
- Follow existing idioms in `src/` (modern C++20 for C++ code).
- Keep public API changes backwards-compatible when possible.
- Add unit tests for bug fixes and new features.

CI
- The repository uses GitHub Actions. New dependencies needed for CI must be added to `.github/workflows/ci.yml` and the relevant install step.

Commit messages
- Use concise imperative messages, e.g. `fix: handle null-alloc in slab allocator` or `feat: add marker logging to server`.

Pull request checklist
- [ ] Tests added/updated
- [ ] Build passes locally
- [ ] CI passes
- [ ] Documentation/README updated if needed

Reporting security issues
- If you discover a security issue, please raise it privately via the repo maintainers rather than opening a public issue.

Contact
- Open an issue or mention `@theonlychant` in PR comments for review requests.
