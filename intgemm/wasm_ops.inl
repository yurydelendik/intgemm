#define WASM_ENABLED_FOR_ION 1
#ifdef WASM_ENABLED_FOR_ION
#define WASM_ENABLED_FOR_WORMHOLE 1
#endif

static inline __m128i _mm_cmpneq_ps(__m128 a, __m128 b) {
  return a != b;
}
static inline __m128i _mm_cmplt_ps(__m128 a, __m128 b) {
  return a < b;
}
static inline __m128i _mm_cmpeq_epi8(__m128i a, __m128i b) {
  return ((__i8x16)a) == ((__i8x16)b);
}
static inline __m128i _mm_cmplt_epi8(__m128i a, __m128i b) {
  return ((__i8x16)a) < ((__i8x16)b);
}
static inline __m128i _mm_cmpgt_epi8(__m128i a, __m128i b) {
  return ((__i8x16)a) > ((__i8x16)b);
}
static inline __m128i _mm_cmpgt_epi16(__m128i a, __m128i b) {
  return ((__i16x8)a) > ((__i16x8)b);
}
static inline __m128i _mm_cmplt_epi32(__m128i a, __m128i b) {
  return ((__i32x4)a) < ((__i32x4)b);
}
static inline __m128i _mm_srli_si128(__m128i a, int c) {
  // psrldq xmm, imm8
  if (c != 4)
    __builtin_trap();
  __i8x16 r = (__i8x16)__builtin_wasm_replace_lane_i32x4((__i32x4)a, /* index = */ 0, 0);
  return __builtin_wasm_shuffle_v8x16(r, r,
      4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3);
}
static inline __m128i _mm_shuffle_epi32(__m128i a, int c) {
  // pshufd xmm, imm8
#define DO_SHUFFLE(c_) if (c == (c_)) { \
  constexpr int __c0 = ((c_) >> 0) & 3; \
  constexpr int __c1 = ((c_) >> 2) & 3; \
  constexpr int __c2 = ((c_) >> 4) & 3; \
  constexpr int __c3 = ((c_) >> 6) & 3; \
  return __builtin_wasm_shuffle_v8x16((__i8x16)a, (__i8x16)a, \
    (__c0)*4, (__c0)*4 + 1, (__c0)*4 + 2, (__c0)*4 + 3, \
    (__c1)*4, (__c1)*4 + 1, (__c1)*4 + 2, (__c1)*4 + 3, \
    (__c2)*4, (__c2)*4 + 1, (__c2)*4 + 2, (__c2)*4 + 3, \
    (__c3)*4, (__c3)*4 + 1, (__c3)*4 + 2, (__c3)*4 + 3); \
}

  DO_SHUFFLE(8)

  __builtin_trap();
#undef DO_SHUFFLE
}
static inline __m128 _mm_shuffle_ps(__m128 a, __m128 b, int c) {
  // shufps xmm, xmm, imm8
#define DO_SHUFFLE(c_) if (c == (c_)) { \
  constexpr int __c0 = ((c_) >> 0) & 3; \
  constexpr int __c1 = ((c_) >> 2) & 3; \
  constexpr int __c2 = ((c_) >> 4) & 3; \
  constexpr int __c3 = ((c_) >> 6) & 3; \
  return __builtin_wasm_shuffle_v8x16((__i8x16)a, (__i8x16)b, \
    (__c0)*4, (__c0)*4 + 1, (__c0)*4 + 2, (__c0)*4 + 3, \
    (__c1)*4, (__c1)*4 + 1, (__c1)*4 + 2, (__c1)*4 + 3, \
    (__c2)*4 + 16, (__c2)*4 + 17, (__c2)*4 + 18, (__c2)*4 + 19, \
    (__c3)*4 + 16, (__c3)*4 + 17, (__c3)*4 + 18, (__c3)*4 + 19); \
}

  DO_SHUFFLE(1)
  DO_SHUFFLE(3 * 4 + 2)

  __builtin_trap();
#undef DO_SHUFFLE
}
static inline __m128 _mm_max_ps(__m128 a, __m128 b) {
  return __builtin_wasm_max_f32x4(a, b);
}
static inline __m128 _mm_add_ps(__m128 a, __m128 b) {
  return a + b;
}
static inline __m128 _mm_loadu_ps(const float* p) {
  struct __wasm_v128_load_struct {
    __m128 v;
  } __attribute__((__packed__, __may_alias__));
  return ((const struct __wasm_v128_load_struct *)p)->v;
}
static inline __m128i _mm_add_epi8(__m128i a, __m128i b) {
  return ((__i8x16)a) + ((__i8x16)b);
}
static inline __m128i _mm_sub_epi8(__m128i a, __m128i b) {
  return ((__i8x16)a) - ((__i8x16)b);
}
static inline __m128 _mm_setzero_ps() {
  return __f32x4{0.f, 0.f, 0.f, 0.f};
}
static inline __m128d _mm_setzero_pd() {
  return __f64x2{0., 0.};
}
static inline __m128 _mm_set1_ps(float a) {
  return __f32x4{a, a, a, a};
}
static inline __m128i _mm_set1_epi8(int8_t a) {
  return __i8x16{a, a, a, a, a, a, a, a,
                 a, a, a, a, a, a, a, a};
}
static inline __m128i _mm_set1_epi16(int16_t a) {
  return __i16x8{a, a, a, a, a, a, a, a};
}
static inline __m128i _mm_set1_epi32(int32_t a) {
  return __i32x4{a, a, a, a};
}
static inline __m128i _mm_packs_epi16(__m128i a, __m128i b) {
  return __builtin_wasm_narrow_s_i8x16_i16x8((__i16x8)a,
                                             (__i16x8)b);
}
static inline __m128i _mm_packs_epi32(__m128i a, __m128i b) {
  return __builtin_wasm_narrow_s_i16x8_i32x4((__i32x4)a,
                                             (__i32x4)b);
}


static inline __m128i unpackhi_epi8(__m128i a, __m128i b) {
    return __builtin_wasm_shuffle_v8x16(
           (__i8x16)a, (__i8x16)b,
           8,24,9,25,10,26,11,27,12,28,13,29,14,30,15,31);
}
static inline __m128i unpacklo_epi8(__m128i a, __m128i b) {
    return __builtin_wasm_shuffle_v8x16(
           (__i8x16)a, (__i8x16)b,
           0,16,1,17,2,18,3,19,4,20,5,21,6,22,7,23);
}
static inline __m128i unpackhi_epi16(__m128i a, __m128i b) {
    return __builtin_wasm_shuffle_v8x16(
           (__i8x16)a, (__i8x16)b,
           8,9,24,25,10,11,26,27,12,13,28,29,14,15,30,31);
}
static inline __m128i unpacklo_epi16(__m128i a, __m128i b) {
    return __builtin_wasm_shuffle_v8x16(
           (__i8x16)a, (__i8x16)b,
           0,1,16,17,2,3,18,19,4,5,20,21,6,7,22,23);
}
static inline __m128i unpackhi_epi32(__m128i a, __m128i b) {
    return __builtin_wasm_shuffle_v8x16(
           (__i8x16)a, (__i8x16)b,
           8,9,10,11,24,25,26,27,12,13,14,15,28,29,30,31);
}
static inline __m128i unpacklo_epi32(__m128i a, __m128i b) {
    return __builtin_wasm_shuffle_v8x16(
           (__i8x16)a, (__i8x16)b,
           0,1,2,3,16,17,18,19,4,5,6,7,20,21,22,23);
}
static inline __m128i unpackhi_epi64(__m128i a, __m128i b) {
    return __builtin_wasm_shuffle_v8x16(
           (__i8x16)a, (__i8x16)b,
           8,9,10,11,12,13,14,15,24,25,26,27,28,29,30,31);
}
static inline __m128i unpacklo_epi64(__m128i a, __m128i b) {
    return __builtin_wasm_shuffle_v8x16(
           (__i8x16)a, (__i8x16)b,
           0,1,2,3,4,5,6,7,16,17,18,19,20,21,22,23);
}

static inline __m128 mul_ps(__m128 a, __m128 b) {
  return a * b;
}
static inline __m128 sub_ps(__m128 a, __m128 b) {
  return a - b;
}
static inline __m128d mul_pd(__m128d a, __m128d b) {
  return a * b;
}
static inline __m128 add_ps(__m128 a, __m128 b) {
  return a + b;
}
static inline __m128d add_pd(__m128d a, __m128d b) {
  return a + b;
}
static inline __m128 max_ps(__m128 a, __m128 b) {
  return __builtin_wasm_max_f32x4(a, b);
}
static inline __m128d max_pd(__m128d a, __m128d b) {
  return __builtin_wasm_max_f64x2(a, b);
}
static inline __m128i cvtps_epi32(__m128 a) {
#ifdef WASM_ENABLED_FOR_ION
  return __builtin_convertvector(
    __builtin_wasm_nearest_f32x4((__f32x4)a), __i32x4);
#else
  return __builtin_convertvector(
    __f32x4{
      nearbyintf(((__f32x4)a)[0]), nearbyintf(((__f32x4)a)[1]),
      nearbyintf(((__f32x4)a)[2]), nearbyintf(((__f32x4)a)[3])}, __i32x4);
#endif
}
static inline __m128i cvttps_epi32(__m128 a) {
  return __builtin_wasm_trunc_saturate_s_i32x4_f32x4((__f32x4)a);
}
static inline __m128 cvtepi32_ps(__m128i a) {
  return __builtin_convertvector((__i32x4)a, __f32x4);
}
static inline __m128i madd_epi16(__m128i a, __m128i b) {
  // pmaddwd xmm, xmm
#ifdef WASM_ENABLED_FOR_WORMHOLE
  return wasm_v8x16_shuffle(a, b, 31, 0, 30, 2, 29, 4, 28, 6, 27, 8, 26, 10, 25, 12, 24, 2 /* PMADDWD */);
#else
  __i32x4 c = ((((__i32x4)a) << 16) >> 16) * ((((__i32x4)b) << 16) >> 16);
  __i32x4 d = (((__i32x4)a) >> 16) * (((__i32x4)b) >> 16);
  return c + d;
#endif
}
static inline __m128i add_epi8(__m128i a, __m128i b) {
  return ((__i8x16)a) + ((__i8x16)b);
}
static inline __m128i add_epi16(__m128i a, __m128i b) {
  return ((__i16x8)a) + ((__i16x8)b);
}
static inline __m128 add_epi32(__m128 a, __m128 b) {
  return ((__i32x4)a) + ((__i32x4)b);
}
static inline __m128i adds_epi16(__m128i a, __m128i b) {
  // paddsw xmm, xmm
  return __builtin_wasm_add_saturate_s_i16x8((__i16x8)a, (__i16x8)b);
}
static inline __m128i maddubs_epi16(__m128i a, __m128i b) {
  // pmaddubsw xmm, xmm
#ifdef WASM_ENABLED_FOR_WORMHOLE
  return wasm_v8x16_shuffle(a, b, 31, 0, 30, 2, 29, 4, 28, 6, 27, 8, 26, 10, 25, 12, 24, 1 /* PMADDUBSW */);
#else
  __i16x8 c =
    ((__i16x8)((((__u16x8)a) << 8) >> 8)) *
    ((((__i16x8)b) << 8) >> 8);
  __i16x8 d =
    ((__i16x8)(((__u16x8)a) >> 8)) *
    (((__i16x8)b) >> 8);
  return __builtin_wasm_add_saturate_s_i16x8(c, d);
#endif
}
static inline __m128 max_epi16(__m128 a, __m128 b) {
  return __builtin_wasm_max_s_i16x8((__i16x8)a, (__i16x8)b);
}
static inline __m128i mullo_epi16(__m128i a, __m128i b) {
  return ((__i16x8)a) * ((__i16x8)b);
}
static inline __m128i mul_epu32(__m128i a, __m128i b) {
  // TODO https://bugzilla.mozilla.org/show_bug.cgi?id=1682466
  // pmuludq xmm, xmm
  return
      __u64x2{((__u32x4)a)[0], ((__u32x4)a)[2]} *
      __u64x2{((__u32x4)b)[0], ((__u32x4)b)[2]};
}
static inline __m128i abs_epi8(__m128i a) {
  return __builtin_wasm_abs_i8x16((__i8x16)a);
}
static inline __m128i sign_epi8(__m128i a, __m128i b) { 
  // psignb xmm, xmm
  return ((__u8x16)(a ^ (((__i8x16)b) >> 7))) + (((__u8x16)b) >> 7);
}
static inline __m128 and_ps(__m128 a, __m128 b) {
  return (((__m128i)a) & ((__m128i)b));
}
static inline __m128i and_si(__m128i a, __m128i b) {
  return a & b;
}
static inline __m128i or_si(__m128i a, __m128i b) {
  return a | b;
}
static inline __m128i xor_si(__m128i a, __m128i b) {
  return a ^ b;
}
template <int imm8> static inline __m128i srli_epi16(__m128i a) {
  return ((__u16x8)a >> imm8);
}
template <int imm8> static inline __m128i slli_epi16(__m128i a) {
  return ((__u16x8)a << imm8);
}

static inline __m128i packs_epi16(__m128i a, __m128i b) {
  return _mm_packs_epi16(a, b);
}
static inline __m128i packs_epi32(__m128i a, __m128i b) {
  return _mm_packs_epi32(a, b);
}
template <> inline __m128i set1_epi8<__m128i>(int8_t to) {
  return _mm_set1_epi8(to);
}
template <> inline __m128i set1_epi16<__m128i>(int16_t to) {
  return _mm_set1_epi16(to);
}
template <> inline __m128i set1_epi32<__m128i>(int32_t to) {
  return _mm_set1_epi32(to);
}
static inline __m128 cast_ps(__m128i a) {
  return (__m128)a;
}
template <> inline __m128 setzero_ps<__m128>() {
  return _mm_setzero_ps();
}
template <> inline __m128d setzero_pd<__m128d>() {
  return _mm_setzero_pd();
}
template <> inline __m128 set1_ps<__m128>(float to) {
  return _mm_set1_ps(to);
}
template <> inline __m128 loadu_ps(const float* mem_addr) {
  return _mm_loadu_ps(mem_addr);
}

#undef WASM_ENABLED_FOR_ION
#undef WASM_ENABLED_FOR_WORMHOLE