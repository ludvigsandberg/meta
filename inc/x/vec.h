#ifndef XVEC_H
#define XVEC_H

#include <stdint.h>
#include <math.h>
#include <assert.h>

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

// number of components in vector
#define xvec_n(V) (sizeof((V).nth) / sizeof((V).nth[0]))

#define xvec_op(OP, A, B, O)                                                  \
    do {                                                                      \
        assert(xvec_n(A) == xvec_n(B) && xvec_n(B) == xvec_n(O) &&            \
               "vec mismatch");                                               \
                                                                              \
        for (int xuniq(i) = 0; xuniq(i) < xvec_n(A); ++xuniq(i)) {            \
            (O).nth[xuniq(i)] = (A).nth[xuniq(i)] OP(B).nth[xuniq(i)];        \
        }                                                                     \
    } while (0)

#define xvec_add(A, B, O) xvec_op(+, A, B, O)

#define xvec_sub(A, B, O) xvec_op(-, A, B, O)

#define xvec_mul(A, B, O) xvec_op(*, A, B, O)

#define xvec_div(A, B, O) xvec_op(/, A, B, O)

#define _xvec_len(T, SQRT, V, O)                                              \
    do {                                                                      \
        T xuniq(len) = (T)0;                                                  \
        for (size_t xuniq(i) = 0; xuniq(i) < xvec_n(V); ++xuniq(i)) {         \
            xuniq(len) += (T)(V).nth[xuniq(i)] * (T)(V).nth[xuniq(i)];        \
        }                                                                     \
        (O) = (SQRT)(xuniq(len));                                             \
    } while (0)

// vec len
#define xvec_len_f32(V, O) _xvec_len(float, sqrtf, V, O)

// vec len
#define xvec_len_f64(V, O) _xvec_len(double, sqrt, V, O)

#define _xvec_norm(T, POSTFIX, V, O)                                          \
    do {                                                                      \
        T xuniq2(len, _xvec_norm);                                            \
        xvec_len_##POSTFIX(V, xuniq2(len, _xvec_norm));                       \
        assert(xuniq2(len, _xvec_norm) != (T)0 && "zero len");                \
                                                                              \
        for (size_t xuniq2(i, _xvec_norm) = 0;                                \
             xuniq2(i, _xvec_norm) < xvec_n(V); ++xuniq2(i, _xvec_norm)) {    \
            (O).nth[xuniq2(i, _xvec_norm)] =                                  \
                (T)(V).nth[xuniq2(i, _xvec_norm)] / xuniq2(len, _xvec_norm);  \
        }                                                                     \
    } while (0)

// normalize vec, o should be xvec<n>f32_t
#define xvec_norm_f32(V, O) _xvec_norm(float, f32, V, O)

// normalize vec, o should be xvec<n>f64_t
#define xvec_norm_f64(V, O) _xvec_norm(double, f64, V, O)

// compute dot product
#define xvec_dot(A, B, O)                                                     \
    do {                                                                      \
        assert(xvec_n(A) == xvec_n(B) && "vec mismatch");                     \
        (O) = 0;                                                              \
        for (size_t xuniq(i) = 0; xuniq(i) < xvec_n(A); ++xuniq(i)) {         \
            (O) += (A).nth[xuniq(i)] * (B).nth[xuniq(i)];                     \
        }                                                                     \
    } while (0)

// compute cross product
#define xvec3_cross(A, B, O)                                                  \
    do {                                                                      \
        /* ay x bz - az x by */                                               \
        (O).xyz.x = (A).xyz.y * (B).xyz.z - (A).xyz.z * (B).xyz.y;            \
        /* az x bx - ax x bz */                                               \
        (O).xyz.y = (A).xyz.z * (B).xyz.x - (A).xyz.x * (B).xyz.z;            \
        /* ax x by - ay x bx */                                               \
        (O).xyz.z = (A).xyz.x * (B).xyz.y - (A).xyz.y * (B).xyz.x;            \
    } while (0)

#endif
