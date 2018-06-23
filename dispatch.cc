#include "dispatch.h"

#include "sse2_gemm.h"
#include "avx2_gemm.h"
#include "avx512_gemm.h"

namespace intgemm {

UnsupportedCPU::UnsupportedCPU() {}

UnsupportedCPU::~UnsupportedCPU() {}

const char *UnsupportedCPU::what() const throw() {
  return "Integer matrix multiplication has not been efficiently implemented for your CPU.";
}

namespace {

struct Unsupported_16bit {
  static void Quantize(const float *, int16_t *, float, int) {
    throw UnsupportedCPU();
  }
  static void PrepareB(const float *, int16_t *, float, int, int) {
    throw UnsupportedCPU();
  }
  static void Multiply(const int16_t *, const int16_t *, float *C, float, int, int, int) {
    throw UnsupportedCPU();
  }
};

struct Unsupported_8bit {
  static void Quantize(const float *, int8_t *, float, int) {
    throw UnsupportedCPU();
  }
  static void PrepareB(const float *, int8_t *, float, int, int) {
    throw UnsupportedCPU();
  }
  static void Multiply(const int8_t *, const int8_t *, float *C, float, int, int, int) {
    throw UnsupportedCPU();
  }
};

/* Returns:
 * avx512 if the CPU supports AVX512F (though really it should be AVX512BW, but
 * cloud providers lie).  TODO: don't catch Knights processors with this.
 *
 * avx2 if the CPU supports AVX2
 * 
 * ssse3 if the CPU supports SSSE3 (this distinction from SSE2 matters for 8-bit)
 * 
 * sse2 if the CPU supports SSE2
 *
 * unsupported otherwise
 */
template <class T> T ChooseCPU(T avx512, T avx2, T ssse3, T sse2, T unsupported) {
  // TODO: don't catch Knights processors here!
  if (__builtin_cpu_supports("avx512f")) {
    return avx512;
  } else if (__builtin_cpu_supports("avx2")) {
    return avx2;
  } else if (__builtin_cpu_supports("ssse3")) {
    return ssse3;
  } else if (__builtin_cpu_supports("sse2")) {
    return sse2;
  } else {
    return unsupported;
  }
}

} // namespace

void (*Dispatch_16bit::Quantize)(const float *input, int16_t *output, float quant_mult, int size) = ChooseCPU(AVX512_16bit::Quantize, AVX2_16bit::Quantize, SSE2_16bit::Quantize, SSE2_16bit::Quantize, Unsupported_16bit::Quantize);
void (*Dispatch_16bit::PrepareB)(const float *input, int16_t *output, float quant_mult, int rows, int cols) = ChooseCPU(AVX512_16bit::PrepareB, AVX2_16bit::PrepareB, SSE2_16bit::PrepareB, SSE2_16bit::PrepareB, Unsupported_16bit::PrepareB);
void (*Dispatch_16bit::Multiply)(const int16_t *A, const int16_t *B, float *C, float unquant_mult, int A_rows, int width, int B_cols) = ChooseCPU(AVX512_16bit::Multiply, AVX2_16bit::Multiply, SSE2_16bit::Multiply, SSE2_16bit::Multiply, Unsupported_16bit::Multiply);

void (*Dispatch_8bit::Quantize)(const float *input, int8_t *output, float quant_mult, int size) = ChooseCPU(AVX512_8bit::Quantize, AVX2_8bit::Quantize, SSSE3_8bit::Quantize, Unsupported_8bit::Quantize, Unsupported_8bit::Quantize);
void (*Dispatch_8bit::PrepareB)(const float *input, int8_t *output, float quant_mult, int rows, int cols) = ChooseCPU(AVX512_8bit::PrepareB, AVX2_8bit::PrepareB, SSSE3_8bit::PrepareB, Unsupported_8bit::PrepareB, Unsupported_8bit::PrepareB);
void (*Dispatch_8bit::Multiply)(const int8_t *A, const int8_t *B, float *C, float unquant_mult, int A_rows, int width, int B_cols) = ChooseCPU(AVX512_8bit::Multiply, AVX2_8bit::Multiply, SSSE3_8bit::Multiply, Unsupported_8bit::Multiply, Unsupported_8bit::Multiply);

} // namespace intgemm