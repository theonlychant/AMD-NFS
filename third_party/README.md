# third_party

Placeholders and integration notes for high-performance kernels.

Suggested contents:

- `flash_attention/` — prebuilt FlashAttention kernels (if available for ROCm)
- `paged_attention/` — prebuilt PagedAttention implementations
- `rocblas/` — optional wrappers or prebuilt rocBLAS binaries

Integration approach:

1. Add a CMake `Find` module or set `ROCBLAS_ROOT` to point at ROCm.
2. When ROCm is present, link against `rocblas` and call `rocblas_sgemm`.
3. Provide small wrapper sources to adapt data layout and pointer modes.
