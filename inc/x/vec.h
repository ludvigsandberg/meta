#ifndef XVEC_H
#define XVEC_H

#include <stdint.h>

#include <x/meta.h>

#define xvec_components_2(T)                                                  \
    struct {                                                                  \
        T x;                                                                  \
        T y;                                                                  \
    } xy;                                                                     \
    struct {                                                                  \
        T x;                                                                  \
        T z;                                                                  \
    } xz;                                                                     \
    struct {                                                                  \
        T y;                                                                  \
        T z;                                                                  \
    } yz;                                                                     \
    struct {                                                                  \
        T u;                                                                  \
        T v;                                                                  \
    } uv;

#define xvec_components_3(T)                                                  \
    struct {                                                                  \
        T x;                                                                  \
        T y;                                                                  \
        T z;                                                                  \
    } xyz;                                                                    \
    struct {                                                                  \
        T r;                                                                  \
        T g;                                                                  \
        T b;                                                                  \
    } rgb;                                                                    \
    struct {                                                                  \
        T u;                                                                  \
        T v;                                                                  \
        T w;                                                                  \
    } uvw;

#define xvec_components_4(T)                                                  \
    struct {                                                                  \
        T x;                                                                  \
        T y;                                                                  \
        T z;                                                                  \
        T w;                                                                  \
    } xyzw;                                                                   \
    struct {                                                                  \
        T r;                                                                  \
        T g;                                                                  \
        T b;                                                                  \
        T a;                                                                  \
    } rgba;                                                                   \
    struct {                                                                  \
        T u;                                                                  \
        T v;                                                                  \
        T s;                                                                  \
        T t;                                                                  \
    } uvst;

#define xvec(N, T)                                                            \
    union {                                                                   \
        xvec_components_##N(T);                                               \
        T nth[N];                                                             \
    }

typedef xvec(2, float) xvec2f32_t;
typedef xvec(2, double) xvec2f64_t;
typedef xvec(2, int8_t) xvec2i8_t;
typedef xvec(2, uint8_t) xvec2u8_t;
typedef xvec(2, int16_t) xvec2i16_t;
typedef xvec(2, uint16_t) xvec2u16_t;
typedef xvec(2, int32_t) xvec2i32_t;
typedef xvec(2, uint32_t) xvec2u32_t;
typedef xvec(2, int64_t) xvec2i64_t;
typedef xvec(2, uint64_t) xvec2u64_t;
typedef xvec(3, float) xvec3f32_t;
typedef xvec(3, double) xvec3f64_t;
typedef xvec(3, int8_t) xvec3i8_t;
typedef xvec(3, uint8_t) xvec3u8_t;
typedef xvec(3, int16_t) xvec3i16_t;
typedef xvec(3, uint16_t) xvec3u16_t;
typedef xvec(3, int32_t) xvec3i32_t;
typedef xvec(3, uint32_t) xvec3u32_t;
typedef xvec(3, int64_t) xvec3i64_t;
typedef xvec(3, uint64_t) xvec3u64_t;
typedef xvec(4, float) xvec4f32_t;
typedef xvec(4, double) xvec4f64_t;
typedef xvec(4, int8_t) xvec4i8_t;
typedef xvec(4, uint8_t) xvec4u8_t;
typedef xvec(4, int16_t) xvec4i16_t;
typedef xvec(4, uint16_t) xvec4u16_t;
typedef xvec(4, int32_t) xvec4i32_t;
typedef xvec(4, uint32_t) xvec4u32_t;
typedef xvec(4, int64_t) xvec4i64_t;
typedef xvec(4, uint64_t) xvec4u64_t;

#define xvec_op(OP, N, A, B, O)                                               \
    do {                                                                      \
        for (int xuniq(i) = 0; xuniq(i) < (N); ++xuniq(i)) {                  \
            (O).nth[xuniq(i)] = (A).nth[xuniq(i)] OP(B).nth[xuniq(i)];        \
        }                                                                     \
    } while (0)

#define xvec_add(N, A, B, O) xvec_op(+, N, A, B, O)

#define xvec_sub(N, A, B, O) xvec_op(-, N, A, B, O)

#define xvec_mul(N, A, B, O) xvec_op(*, N, A, B, O)

#define xvec_div(N, A, B, O) xvec_op(/, N, A, B, O)

#endif
