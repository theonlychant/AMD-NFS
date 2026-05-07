#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int launch_gemm(const void* A, const void* B, void* C,
               int M, int N, int K);

#ifdef __cplusplus
}
#endif
