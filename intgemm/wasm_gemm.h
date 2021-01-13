#pragma once

#include "kernels.h"
#include "multiply.h"
#include "types.h"

#include <cstdint>

namespace intgemm {
namespace Wasm {

INTGEMM_WASM inline __m128i QuantizerGrab(const float *input, const __m128 quant_mult_reg) {
  return kernels::quantize(loadu_ps<__m128>(input), quant_mult_reg);
}

INTGEMM_SELECT_COL_B(INTGEMM_WASM, __m128i)

// 8 bit 

class QuantizeTile8 {
  public:
    INTGEMM_WASM static inline Register ForReshape(FRegister mult_reg, const float *input, Index cols) {
      // Skip a row.
      return Tile(mult_reg, input, input + 4, input + 2 * cols, input + 2 * cols + 4);
    }

    INTGEMM_WASM static inline Register Consecutive(FRegister mult_reg, const float *input) {
      return Tile(mult_reg, input, input + 4, input + 8, input + 12);
    }

    INTGEMM_WASM static inline Register ConsecutiveU(FRegister mult_reg, const float *input) {
      return TileU(mult_reg, input, input + 4, input + 8, input + 12);
    }

    INTGEMM_WASM static inline Register ConsecutiveWithWrapping(FRegister mult_reg, const float *input, Index cols_left, Index cols, Index row_step) {
      const float* inputs[4];
      for (Index i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i) {
        while (cols_left < sizeof(Register) / sizeof(float)) {
          input += cols * (row_step - 1);
          cols_left += cols;
        }
        inputs[i] = input;
        input += sizeof(Register) / sizeof(float);
        cols_left -= sizeof(Register) / sizeof(float);
      }
      return Tile(mult_reg, inputs[0], inputs[1], inputs[2], inputs[3]);
    }

    // Quantize 16xfloat into 16xint8_t
    INTGEMM_WASM static inline __m128i Tile(FRegister mult_reg, const float *input0, const float *input1, const float *input2, const float *input3) {
      const __m128i neg127 = wasm_i8x16_splat(-127);
      __m128i g0 = QuantizerGrab(input0, mult_reg);
      __m128i g1 = QuantizerGrab(input1, mult_reg);
      __m128i g2 = QuantizerGrab(input2, mult_reg);
      __m128i g3 = QuantizerGrab(input3, mult_reg);
      __m128i packed0 = packs_epi32(g0, g1);
      __m128i packed1 = packs_epi32(g2, g3);
      __m128i packed = packs_epi16(packed0, packed1);
      // Ban -128.
      return wasm_i8x16_max(packed, neg127);
      // No permute needed.  packs is in order for WASM.
    }

  private:
    INTGEMM_WASM static inline __m128i TileU(FRegister mult_reg, const float *input0, const float *input1, const float *input2, const float *input3) {
      const __m128i neg127 = wasm_i8x16_splat(-127);
      const __u16x8 pos127 = wasm_i8x16_splat(127);
      __m128i g0 = QuantizerGrab(input0, mult_reg);
      __m128i g1 = QuantizerGrab(input1, mult_reg);
      __m128i g2 = QuantizerGrab(input2, mult_reg);
      __m128i g3 = QuantizerGrab(input3, mult_reg);
      __m128i packed0 = packs_epi32(g0, g1);
      __m128i packed1 = packs_epi32(g2, g3);
      __m128i packed = packs_epi16(packed0, packed1);
      // Ban -128.
      return ((__u16x8)wasm_i8x16_max(packed, neg127)) + pos127;
      // No permute needed.  packs is in order for WASM.
    }
};

struct Kernels8 {
  typedef int8_t Integer;

  // Currently A is prepared by quantization but this could theoretically change.
  INTGEMM_WASM static inline void PrepareA(const float *input, int8_t *output, float quant_mult, Index rows, Index cols) {
    Quantize(input, output, quant_mult, rows * cols);
  }

 private:
  INTGEMM_QUANTIZE_THREAD(INTGEMM_WASM)
 public:
  INTGEMM_QUANTIZE(INTGEMM_WASM)

  // Version with unsigned int + 127
  // Currently A is prepared by quantization but this could theoretically change.
  INTGEMM_WASM static inline void PrepareA(const float *input, uint8_t *output, float quant_mult, Index rows, Index cols) {
    QuantizeU(input, output, quant_mult, rows * cols);
  }

  INTGEMM_WASM static void QuantizeU(const float *input, uint8_t *output, float quant_mult, Index size) {
    assert(size % 16 == 0);
    assert(reinterpret_cast<uintptr_t>(input) % 16 == 0);
    assert(reinterpret_cast<uintptr_t>(output) % 16 == 0);
    FRegister q = set1_ps<FRegister>(quant_mult);
    const float *end = input + size;
    for (; input != end; input += 16, output += 16) {
      *reinterpret_cast<__m128i*>(output) = QuantizeTile8::ConsecutiveU(q, input);
    }
  }

  // Tile size for B; B must be a multiple of this block size.
  static const Index kBTileRow = 16;
  static const Index kBTileCol = 8;

  INTGEMM_PREPARE_B_8(INTGEMM_WASM, Wasm::QuantizeTile8)
  INTGEMM_PREPARE_B_QUANTIZED_TRANSPOSED(INTGEMM_WASM, int8_t)
  INTGEMM_PREPARE_B_TRANSPOSED(INTGEMM_WASM, QuantizeTile8, int8_t)

  INTGEMM_WASM static void SelectColumnsB(const int8_t *input, int8_t *output, Index rows, const Index *cols_begin, const Index *cols_end) {
    Wasm::SelectColumnsOfB((const __m128i*)input, (__m128i*)output, rows, cols_begin, cols_end);
  }

  INTGEMM_MULTIPLY8(__m128i, INTGEMM_WASM, CPUType::WASM)

  INTGEMM_MULTIPLY8SHIFT(__m128i, INTGEMM_WASM, CPUType::WASM)

  INTGEMM_PREPAREBIASFOR8(__m128i, INTGEMM_WASM, CPUType::WASM)

  constexpr static const char *const kName = "8-bit Wasm";

  static const CPUType kUses = CPUType::WASM;
};

// 16 bit

class QuantizeTile16 {
  public:
    INTGEMM_WASM static inline Register Consecutive(__m128 mult_reg, const float *input) {
      return Tile(mult_reg, input, input + 4);
    }

    INTGEMM_WASM static inline Register ConsecutiveWithWrapping(__m128 mult_reg, const float *input, Index cols_left, Index cols, Index row_step) {
      return Tile(mult_reg,
        input,
        input + 4 + (cols_left <= 4 ? cols * (row_step - 1) : 0));
    }

    INTGEMM_WASM static inline Register ForReshape(__m128 mult_reg, const float *input, int) {
      return Consecutive(mult_reg, input);
    }

  private:
    INTGEMM_WASM static inline Register Tile(__m128 mult_reg, const float *input0, const float *input1) {
      __m128i g0 = kernels::quantize(loadu_ps<__m128>(input0), mult_reg);
      __m128i g1 = kernels::quantize(loadu_ps<__m128>(input1), mult_reg);
      return packs_epi32(g0, g1);
    }
};

// This should be pure WASM (and below).
struct Kernels16 {
  typedef int16_t Integer;

  // Currently A is prepared by quantization but this could theoretically change.
  INTGEMM_WASM static inline void PrepareA(const float *input, int16_t *output, float quant_mult, Index rows, Index cols) {
    Quantize(input, output, quant_mult, rows * cols);
  }

  INTGEMM_WASM static void Quantize(const float *input, int16_t *output, float quant_mult, Index size) {
    assert(size % 8 == 0);
    assert(reinterpret_cast<uintptr_t>(input) % 16 == 0);
    assert(reinterpret_cast<uintptr_t>(output) % 16 == 0);
    FRegister q = set1_ps<FRegister>(quant_mult);
    const float *end = input + size;
    for (; input != end; input += 8, output += 8) {
      *reinterpret_cast<__m128i*>(output) = QuantizeTile16::Consecutive(q, input);
    }
  }

  // Tile size for B; B must be a multiple of this block size.
  static const Index kBTileRow = 8;
  static const Index kBTileCol = 8;

  INTGEMM_PREPARE_B_16(INTGEMM_WASM, QuantizeTile16)
  INTGEMM_PREPARE_B_QUANTIZED_TRANSPOSED(INTGEMM_WASM, int16_t)
  INTGEMM_PREPARE_B_TRANSPOSED(INTGEMM_WASM, QuantizeTile16, int16_t)

  INTGEMM_WASM static void SelectColumnsB(const int16_t *input, int16_t *output, Index rows, const Index *cols_begin, const Index *cols_end) {
    //TODO #DEFINE
    SelectColumnsOfB((const __m128i*)input, (__m128i*)output, rows * 2, cols_begin, cols_end);
  }
  INTGEMM_MULTIPLY16(__m128i, INTGEMM_WASM, CPUType::WASM)

  constexpr static const char *const kName = "16-bit Wasm";

  static const CPUType kUses = CPUType::WASM;
};

} // namespace Wasm
} // namespace intgemm
