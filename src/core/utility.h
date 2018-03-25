#pragma once
///
#include <cstdio>
#include <immintrin.h>
#include <cmath>
///
#define ROVE rove
#define COMPUTE ROVE.compute
#define DISPLAY_DEVICE COMPUTE->display_device
#define SCENE ROVE.scene
#define TREE ROVE.tree
#define FLIP_FLOP SCENE.flip_flop
#define THREAD_POOL ROVE.thread_pool
///
#define LOG(M, ...) if (ROVE.log) { \
                      if (ftell(ROVE.log) < (1024 * 128)) { \
                        fprintf(ROVE.log, M "\n", ##__VA_ARGS__);\
                        fflush(ROVE.log); \
                      } \
                    }
#define ERR(M, ...) if (ROVE.log) { \
                      if (ftell(ROVE.log) < (1024 * 128)) { \
                        fprintf(ROVE.log, "** Error: " M " : %s : %i\n", \
                        ##__VA_ARGS__, __FILE__, __LINE__); \
                        fflush(ROVE.log); \
                      } \
                    }
///
#if defined (_MSC_VER) || defined (__INTEL_COMPILER)
  #define BSR32(A, R) __asm__("bsr %1, %0" : \
                              "=r" (R) : \
                              "r"  (A))
  #define BSF32(A, R) __asm__("bsf %1, %0" : \
                              "=r" (R) : \
                              "r"  (A))
  #define POPCNT32(A, R) __asm__("popcnt %1, %0" : \
                                 "=r" (R) : \
                                 "ri" (A))
  #define F4A __declspec(align(16)) float
#elif defined(__GNUC__) || defined(__clang__)
  #define BSR32(A, R) __asm__("bsr %1, %0" : \
                              "=r" (R) : \
                              "r"  (A))
  #define BSF32(A, R) __asm__("bsf %1, %0" : \
                              "=r" (R) : \
                              "r"  (A))
  #define POPCNT32(A, R) __asm__("popcnt %1, %0" : \
                                 "=r" (R) : \
                                 "ri" (A))
  #define F4A __attribute__((__aligned__(16))) float
#endif
///
#define RANDOM_U() COMPUTE->random_udist(COMPUTE->random_generator)
#define RANDOM_F() COMPUTE->random_fdist(COMPUTE->random_generator)
///
#define PI 3.14159265359f
///
#define SET_IDENTITY_MATRIX(M) \
{ \
  M[0] = 1.0f; M[4] = 0.0f; M[8] = 0.0f;  M[12] = 0.0f; \
  M[1] = 0.0f; M[5] = 1.0f; M[9] = 0.0f;  M[13] = 0.0f; \
  M[2] = 0.0f; M[6] = 0.0f; M[10] = 1.0f; M[14] = 0.0f; \
  M[3] = 0.0f; M[7] = 0.0f; M[11] = 0.0f; M[15] = 1.0f; \
}
///
#define MULTIPLY_MATRIX(A, B, R) \
{ \
  R[0] = A[0] * B[0] + A[4] * B[1] + A[8] * B[2] + A[12] * B[3]; \
  R[1] = A[1] * B[0] + A[5] * B[1] + A[9] * B[2] + A[13] * B[3]; \
  R[2] = A[2] * B[0] + A[6] * B[1] + A[10] * B[2] + A[14] * B[3]; \
  R[3] = A[3] * B[0] + A[7] * B[1] + A[11] * B[2] + A[15] * B[3]; \
  R[4] = A[0] * B[4] + A[4] * B[5] + A[8] * B[6] + A[12] * B[7]; \
  R[5] = A[1] * B[4] + A[5] * B[5] + A[9] * B[6] + A[13] * B[7]; \
  R[6] = A[2] * B[4] + A[6] * B[5] + A[10] * B[6] + A[14] * B[7]; \
  R[7] = A[3] * B[4] + A[7] * B[5] + A[11] * B[6] + A[15] * B[7]; \
  R[8] = A[0] * B[8] + A[4] * B[9] + A[8] * B[10] + A[12] * B[11]; \
  R[9] = A[1] * B[8] + A[5] * B[9] + A[9] * B[10] + A[13] * B[11]; \
  R[10] = A[2] * B[8] + A[6] * B[9] + A[10] * B[10] + A[14] * B[11]; \
  R[11] = A[3] * B[8] + A[7] * B[9] + A[11] * B[10] + A[15] * B[11]; \
  R[12] = A[0] * B[12] + A[4] * B[13] + A[8] * B[14] + A[12] * B[15]; \
  R[13] = A[1] * B[12] + A[5] * B[13] + A[9] * B[14] + A[13] * B[15]; \
  R[14] = A[2] * B[12] + A[6] * B[13] + A[10] * B[14] + A[14] * B[15]; \
  R[15] = A[3] * B[12] + A[7] * B[13] + A[11] * B[14] + A[15] * B[15]; \
}
///
#define INVERT_MATRIX(A, R) \
{ \
  float inv[16], det; \
  inv[0] = A[5] * A[10] * A[15] - A[5]  * A[11] * A[14] - \
           A[9]  * A[6]  * A[15] + A[9]  * A[7]  * A[14] + \
           A[13] * A[6]  * A[11] - A[13] * A[7]  * A[10]; \
  inv[4] = -A[4]  * A[10] * A[15] + A[4]  * A[11] * A[14] + \
            A[8]  * A[6]  * A[15] - A[8]  * A[7]  * A[14] - \
            A[12] * A[6]  * A[11] + A[12] * A[7]  * A[10]; \
  inv[8] = A[4]  * A[9] * A[15] - A[4]  * A[11] * A[13] - \
           A[8]  * A[5] * A[15] + A[8]  * A[7] * A[13] + \
           A[12] * A[5] * A[11] - A[12] * A[7] * A[9]; \
  inv[12] = -A[4]  * A[9] * A[14] + A[4]  * A[10] * A[13] + \
             A[8]  * A[5] * A[14] - A[8]  * A[6] * A[13] - \
             A[12] * A[5] * A[10] + A[12] * A[6] * A[9]; \
  inv[1] = -A[1]  * A[10] * A[15] + A[1]  * A[11] * A[14] + \
            A[9]  * A[2] * A[15] - A[9]  * A[3] * A[14] - \
            A[13] * A[2] * A[11] + A[13] * A[3] * A[10]; \
  inv[5] = A[0]  * A[10] * A[15] - A[0]  * A[11] * A[14] - \
           A[8]  * A[2] * A[15] + A[8]  * A[3] * A[14] + \
           A[12] * A[2] * A[11] - A[12] * A[3] * A[10]; \
  inv[9] = -A[0]  * A[9] * A[15] + A[0]  * A[11] * A[13] + \
            A[8]  * A[1] * A[15] - A[8]  * A[3] * A[13] - \
            A[12] * A[1] * A[11] + A[12] * A[3] * A[9]; \
  inv[13] = A[0]  * A[9] * A[14] - A[0]  * A[10] * A[13] - \
            A[8]  * A[1] * A[14] + A[8]  * A[2] * A[13] + \
            A[12] * A[1] * A[10] - A[12] * A[2] * A[9]; \
  inv[2] = A[1]  * A[6] * A[15] - A[1]  * A[7] * A[14] - \
           A[5]  * A[2] * A[15] + A[5]  * A[3] * A[14] + \
           A[13] * A[2] * A[7] - A[13] * A[3] * A[6]; \
  inv[6] = -A[0]  * A[6] * A[15] + A[0]  * A[7] * A[14] + \
            A[4]  * A[2] * A[15] - A[4]  * A[3] * A[14] - \
            A[12] * A[2] * A[7] + A[12] * A[3] * A[6]; \
  inv[10] = A[0]  * A[5] * A[15] - A[0]  * A[7] * A[13] - \
            A[4]  * A[1] * A[15] + A[4]  * A[3] * A[13] + \
            A[12] * A[1] * A[7] - A[12] * A[3] * A[5]; \
  inv[14] = -A[0]  * A[5] * A[14] + A[0]  * A[6] * A[13] + \
             A[4]  * A[1] * A[14] - A[4]  * A[2] * A[13] - \
             A[12] * A[1] * A[6] + A[12] * A[2] * A[5]; \
  inv[3] = -A[1] * A[6] * A[11] + A[1] * A[7] * A[10] + \
            A[5] * A[2] * A[11] - A[5] * A[3] * A[10] - \
            A[9] * A[2] * A[7] + A[9] * A[3] * A[6]; \
  inv[7] = A[0] * A[6] * A[11] - A[0] * A[7] * A[10] - \
           A[4] * A[2] * A[11] + A[4] * A[3] * A[10] + \
           A[8] * A[2] * A[7] - A[8] * A[3] * A[6]; \
  inv[11] = -A[0] * A[5] * A[11] + A[0] * A[7] * A[9] + \
             A[4] * A[1] * A[11] - A[4] * A[3] * A[9] - \
             A[8] * A[1] * A[7] + A[8] * A[3] * A[5]; \
  inv[15] = A[0] * A[5] * A[10] - A[0] * A[6] * A[9] - \
            A[4] * A[1] * A[10] + A[4] * A[2] * A[9] + \
            A[8] * A[1] * A[6] - A[8] * A[2] * A[5]; \
  det = A[0] * inv[0] + A[1] * inv[4] + A[2] * inv[8] + A[3] * inv[12]; \
  if (det == 0) {\
    R[0] = 1.0f; R[4] = 0.0f; R[8] = 0.0f; R[12] = 0.0f; \
    R[1] = 0.0f; R[5] = 1.0f; R[9] = 0.0f; R[13] = 0.0f; \
    R[2] = 0.0f; R[6] = 0.0f; R[10] = 1.0f; R[14] = 0.0f; \
    R[3] = 0.0f; R[7] = 0.0f; R[11] = 0.0f; R[15] = 1.0f; \
  } else { \
    det = 1.0f / det; \
    for (uint32_t i = 0; i < 16; ++i) { \
        R[i] = inv[i] * det; \
    } \
  } \
}
///
#define MERGE_BOUNDS(A, B, R) \
{ \
  R[0] = (std::min)(A[0], B[0]); \
  R[1] = (std::min)(A[1], B[1]); \
  R[2] = (std::min)(A[2], B[2]); \
  R[3] = (std::max)(A[3], B[3]); \
  R[4] = (std::max)(A[4], B[4]); \
  R[5] = (std::max)(A[5], B[5]); \
}
///
///
#define AREA_OF_BOUNDS(B, A) \
{ \
  float w = B[3] - B[0]; \
  float h = B[4] - B[1]; \
  float d = B[5] - B[2]; \
  A = (w * h * 2.0f) + (d * h * 2.0f) + (d * w * 2.0f); \
}
///
#define COPY_BOUNDS(A, B) memcpy(B, A, sizeof(float) * 6)
///
#define MIN_TO_BOUNDS(A, B) \
{ \
  B[0] = B[3] = A[0]; \
  B[1] = B[4] = A[1]; \
  B[2] = B[5] = A[2]; \
}
///
#define NEXT_POWER_OF_TWO(VALUE, RESULT) \
{ \
  uint32_t temp = (VALUE + (VALUE == 0)); \
  uint32_t highest_bit; \
  BSR32(temp, highest_bit); \
  RESULT = (1 << (highest_bit + 1)); \
}
///
#define STRING_TO_CHARS(STRING, CHARS) \
{ \
  CHARS = (char*)malloc(sizeof(char) * (STRING.size() + 1)); \
  strcpy(CHARS, STRING.c_str()); \
}
///
#define VECTOR_LENGTH(v) (sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]))
///
#define F4 __m128
#define F4_LOAD(A) _mm_load_ps(A)
#define F4_STORE(A, B) _mm_store_ps(B, A)
#define F4_SET1(A) _mm_set1_ps(A)
#define F4_SET(A, B, C, D) _mm_setr_ps(A, B, C, D)
#define F4_PERMUTE(A, B) _mm_shuffle_ps(A, A, B)
#define F4_BLEND(A, B, C) _mm_blend_ps(A, B, C)
#define F4_MIN(A, B) _mm_min_ps(A, B)
#define F4_MAX(A, B) _mm_max_ps(A, B)
#define F4_ADD(A, B) _mm_add_ps(A, B)
#define F4_SUB(A, B) _mm_sub_ps(A, B)
#define F4_MUL(A, B) _mm_mul_ps(A, B)
#define F4_DIV(A, B) _mm_div_ps(A, B)
#define F4_NEG(A) _mm_xor_ps(_mm_set1_ps(-0.0f), A)
#define F4_NORMALIZE(A) _mm_div_ps(A, _mm_sqrt_ps(_mm_dp_ps(A, A, 0x7F)))
#define XXXX _MM_SHUFFLE(0, 0, 0, 0)
#define YYYY _MM_SHUFFLE(1, 1, 1, 1)
#define ZZZZ _MM_SHUFFLE(2, 2, 2, 2)
#define WWWW _MM_SHUFFLE(3, 3, 3, 3)
#define YZXW _MM_SHUFFLE(3, 0, 2, 1)
