#ifndef X_MAT_H
#define X_MAT_H

#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include <x/meta.h>
#include <x/vec.h>

// row major matrix
#define xmat(R, C, T)                                                         \
    union {                                                                   \
        T at[R][C];                                                           \
        T nth[(R) * (C)];                                                     \
    }

typedef xmat(3, 3, float) xmat3f32_t;
typedef xmat(3, 3, double) xmat3f64_t;
typedef xmat(4, 4, float) xmat4f32_t;
typedef xmat(4, 4, double) xmat4f64_t;

#define xmat_rows(M) (sizeof((M).at) / sizeof((M).at[0]))

#define xmat_cols(M) (sizeof((M).at[0]) / sizeof((M).at[0][0]))

#define xmat_zero(M) memset((M).nth, 0, sizeof((M).nth))

#define _xmat_identity(M, T)                                                  \
    do {                                                                      \
        assert(xmat_rows(M) == xmat_cols(M));                                 \
        xmat_zero(M);                                                         \
        for (size_t xuniq(i) = 0; xuniq(i) < xmat_rows(M); ++xuniq(i)) {      \
            (M).at[xuniq(i)][xuniq(i)] = (T)1;                                \
        }                                                                     \
    } while (0)

#define xmat_identity_f32(M) _xmat_identity(M, float)

#define xmat_identity_f64(M) _xmat_identity(M, double)

// mul (row x k) by (k x col) into (row x col)
#define xmat_mul(R, K, C, A, B, O)                                            \
    do {                                                                      \
        for (int xuniq(r) = 0; xuniq(r) < (R); ++xuniq(r))                    \
            for (int xuniq(c) = 0; xuniq(c) < (C); ++xuniq(c)) {              \
                (O).at[xuniq(r)][xuniq(c)] = 0;                               \
                                                                              \
                for (int xuniq(k) = 0; xuniq(k) < (K); xuniq(k)++) {          \
                    (O).at[xuniq(r)][xuniq(c)] +=                             \
                        (A).at[xuniq(r)][xuniq(k)] *                          \
                        (B).at[xuniq(k)][xuniq(c)];                           \
                }                                                             \
            }                                                                 \
    } while (0)

#define xmat3_mul(A, B, O) xmat_mul(3, 3, 3, A, B, O)

#define xmat4_mul(A, B, O) xmat_mul(4, 4, 4, A, B, O)

#define _xmat4_translate(M, T, VEC, O)                                        \
    do {                                                                      \
        xmat(4, 4, T) xuniq2(m, _xmat4_translate);                            \
        _xmat_identity(xuniq2(m, _xmat4_translate), T);                       \
        (xuniq2(m, _xmat4_translate)).at[0][3] = (VEC).nth[0];                \
        (xuniq2(m, _xmat4_translate)).at[1][3] = (VEC).nth[1];                \
        (xuniq2(m, _xmat4_translate)).at[2][3] = (VEC).nth[2];                \
        xmat4_mul(M, xuniq2(m, _xmat4_translate), O);                         \
    } while (0)

#define xmat4f32_translate(M, VEC, O) _xmat4_translate(M, float, VEC, O)

#define xmat4f64_translate(M, VEC, O) _xmat4_translate(M, double, VEC, O)

#define _xlook_at(T, POS, TARGET, UP, O)                                      \
    do {                                                                      \
        xvec(3, T) xuniq(diff);                                               \
        xvec_sub(TARGET, POS, xuniq(diff));                                   \
                                                                              \
        xvec(3, T) xuniq(forward);                                            \
        xvec_norm_f32(xuniq(diff), xuniq(forward));                           \
                                                                              \
        xvec(3, T) xuniq(right);                                              \
        xvec3_cross(xuniq(forward), UP, xuniq(right));                        \
        xvec(3, T) xuniq(right_norm);                                         \
        xvec_norm_f32(xuniq(right), xuniq(right_norm));                       \
                                                                              \
        xvec(3, T) xuniq(local_up);                                           \
        xvec3_cross(xuniq(right_norm), xuniq(forward), xuniq(local_up));      \
        xvec(3, T) xuniq(local_up_norm);                                      \
        xvec_norm_f32(xuniq(local_up), xuniq(local_up_norm));                 \
                                                                              \
        _xmat_identity(O, T);                                                 \
        (O).at[0][0] = xuniq(right_norm).xyz.x;                               \
        (O).at[1][0] = xuniq(right_norm).xyz.y;                               \
        (O).at[2][0] = xuniq(right_norm).xyz.z;                               \
        (O).at[0][1] = xuniq(local_up_norm).xyz.x;                            \
        (O).at[1][1] = xuniq(local_up_norm).xyz.y;                            \
        (O).at[2][1] = xuniq(local_up_norm).xyz.z;                            \
        (O).at[0][2] = -xuniq(forward).xyz.x;                                 \
        (O).at[1][2] = -xuniq(forward).xyz.y;                                 \
        (O).at[2][2] = -xuniq(forward).xyz.z;                                 \
                                                                              \
        T xuniq(dot);                                                         \
        xvec_dot(xuniq(right_norm), POS, xuniq(dot));                         \
        (O).at[0][3] = -xuniq(dot);                                           \
                                                                              \
        xvec_dot(xuniq(local_up_norm), POS, xuniq(dot));                      \
        (O).at[1][3] = -xuniq(dot);                                           \
                                                                              \
        xvec_dot(xuniq(forward), POS, xuniq(dot));                            \
        (O).at[2][3] = -xuniq(dot);                                           \
    } while (0)

#define xlook_at_f32(POS, TARGET, UP, O) _xlook_at(float, POS, TARGET, UP, O)

#define xlook_at_f64(POS, TARGET, UP, O) _xlook_at(double, POS, TARGET, UP, O)

#define _xperspective(T, TAN, ASPECT, FOV, NEAR, FAR, O)                      \
    do {                                                                      \
        T xuniq(half_tan)  = (TAN)((FOV) / (T)2);                             \
        T xuniq(f_minus_n) = (FAR) - (NEAR);                                  \
                                                                              \
        xmat_zero(O);                                                         \
        (O).at[0][0] = (T)1 / (xuniq(half_tan) * (ASPECT));                   \
        (O).at[1][1] = (T)1 / xuniq(half_tan);                                \
        (O).at[2][2] = -((FAR) + (NEAR)) / xuniq(f_minus_n);                  \
        (O).at[2][3] = -(T)1;                                                 \
        (O).at[3][2] = -((T)2 * (FAR) * (NEAR)) / xuniq(f_minus_n);           \
        (O).at[3][3] = (T)0;                                                  \
    } while (0)

#define xperspective_f32(ASPECT, FOV, NEAR, FAR, O)                           \
    _xperspective(float, tanf, ASPECT, FOV, NEAR, FAR, O)

#define xperspective_f64(ASPECT, FOV, NEAR, FAR, O)                           \
    _xperspective(double, tan, ASPECT, FOV, NEAR, FAR, O)

#define _xmat4_invert(T, M, O)                                                \
    do {                                                                      \
        T xuniq(a2323) =                                                      \
            (M).at[2][2] * (M).at[3][3] - (M).at[2][3] * (M).at[3][2];        \
        T xuniq(a1323) =                                                      \
            (M).at[2][1] * (M).at[3][3] - (M).at[2][3] * (M).at[3][1];        \
        T xuniq(a1223) =                                                      \
            (M).at[2][1] * (M).at[3][2] - (M).at[2][2] * (M).at[3][1];        \
        T xuniq(a0323) =                                                      \
            (M).at[2][0] * (M).at[3][3] - (M).at[2][3] * (M).at[3][0];        \
        T xuniq(a0223) =                                                      \
            (M).at[2][0] * (M).at[3][2] - (M).at[2][2] * (M).at[3][0];        \
        T xuniq(a0123) =                                                      \
            (M).at[2][0] * (M).at[3][1] - (M).at[2][1] * (M).at[3][0];        \
        T xuniq(a2313) =                                                      \
            (M).at[1][2] * (M).at[3][3] - (M).at[1][3] * (M).at[3][2];        \
        T xuniq(a1313) =                                                      \
            (M).at[1][1] * (M).at[3][3] - (M).at[1][3] * (M).at[3][1];        \
        T xuniq(a1213) =                                                      \
            (M).at[1][1] * (M).at[3][2] - (M).at[1][2] * (M).at[3][1];        \
        T xuniq(a2312) =                                                      \
            (M).at[1][2] * (M).at[2][3] - (M).at[1][3] * (M).at[2][2];        \
        T xuniq(a1312) =                                                      \
            (M).at[1][1] * (M).at[2][3] - (M).at[1][3] * (M).at[2][1];        \
        T xuniq(a1212) =                                                      \
            (M).at[1][1] * (M).at[2][2] - (M).at[1][2] * (M).at[2][1];        \
        T xuniq(a0313) =                                                      \
            (M).at[1][0] * (M).at[3][3] - (M).at[1][3] * (M).at[3][0];        \
        T xuniq(a0213) =                                                      \
            (M).at[1][0] * (M).at[3][2] - (M).at[1][2] * (M).at[3][0];        \
        T xuniq(a0312) =                                                      \
            (M).at[1][0] * (M).at[2][3] - (M).at[1][3] * (M).at[2][0];        \
        T xuniq(a0212) =                                                      \
            (M).at[1][0] * (M).at[2][2] - (M).at[1][2] * (M).at[2][0];        \
        T xuniq(a0113) =                                                      \
            (M).at[1][0] * (M).at[3][1] - (M).at[1][1] * (M).at[3][0];        \
        T xuniq(a0112) =                                                      \
            (M).at[1][0] * (M).at[2][1] - (M).at[1][1] * (M).at[2][0];        \
                                                                              \
        T xuniq(det) = (M).at[0][0] * ((M).at[1][1] * xuniq(a2323) -          \
                                       (M).at[1][2] * xuniq(a1323) +          \
                                       (M).at[1][3] * xuniq(a1223)) -         \
                       (M).at[0][1] * ((M).at[1][0] * xuniq(a2323) -          \
                                       (M).at[1][2] * xuniq(a0323) +          \
                                       (M).at[1][3] * xuniq(a0223)) +         \
                       (M).at[0][2] * ((M).at[1][0] * xuniq(a1323) -          \
                                       (M).at[1][1] * xuniq(a0323) +          \
                                       (M).at[1][3] * xuniq(a0123)) -         \
                       (M).at[0][3] * ((M).at[1][0] * xuniq(a1223) -          \
                                       (M).at[1][1] * xuniq(a0223) +          \
                                       (M).at[1][2] * xuniq(a0123));          \
                                                                              \
        assert(xuniq(det) > (T)0 && "matrix not invertible");                 \
        xuniq(det) = (T)1 / xuniq(det);                                       \
                                                                              \
        (O).at[0][0] = xuniq(det) * ((M).at[1][1] * xuniq(a2323) -            \
                                     (M).at[1][2] * xuniq(a1323) +            \
                                     (M).at[1][3] * xuniq(a1223));            \
        (O).at[0][1] = xuniq(det) * -((M).at[0][1] * xuniq(a2323) -           \
                                      (M).at[0][2] * xuniq(a1323) +           \
                                      (M).at[0][3] * xuniq(a1223));           \
        (O).at[0][2] = xuniq(det) * ((M).at[0][1] * xuniq(a2313) -            \
                                     (M).at[0][2] * xuniq(a1313) +            \
                                     (M).at[0][3] * xuniq(a1213));            \
        (O).at[0][3] = xuniq(det) * -((M).at[0][1] * xuniq(a2312) -           \
                                      (M).at[0][2] * xuniq(a1312) +           \
                                      (M).at[0][3] * xuniq(a1212));           \
        (O).at[1][0] = xuniq(det) * -((M).at[1][0] * xuniq(a2323) -           \
                                      (M).at[1][2] * xuniq(a0323) +           \
                                      (M).at[1][3] * xuniq(a0223));           \
        (O).at[1][1] = xuniq(det) * ((M).at[0][0] * xuniq(a2323) -            \
                                     (M).at[0][2] * xuniq(a0323) +            \
                                     (M).at[0][3] * xuniq(a0223));            \
        (O).at[1][2] = xuniq(det) * -((M).at[0][0] * xuniq(a2313) -           \
                                      (M).at[0][2] * xuniq(a0313) +           \
                                      (M).at[0][3] * xuniq(a0213));           \
        (O).at[1][3] = xuniq(det) * ((M).at[0][0] * xuniq(a2312) -            \
                                     (M).at[0][2] * xuniq(a0312) +            \
                                     (M).at[0][3] * xuniq(a0212));            \
        (O).at[2][0] = xuniq(det) * ((M).at[1][0] * xuniq(a1323) -            \
                                     (M).at[1][1] * xuniq(a0323) +            \
                                     (M).at[1][3] * xuniq(a0123));            \
        (O).at[2][1] = xuniq(det) * -((M).at[0][0] * xuniq(a1323) -           \
                                      (M).at[0][1] * xuniq(a0323) +           \
                                      (M).at[0][3] * xuniq(a0123));           \
        (O).at[2][2] = xuniq(det) * ((M).at[0][0] * xuniq(a1313) -            \
                                     (M).at[0][1] * xuniq(a0313) +            \
                                     (M).at[0][3] * xuniq(a0113));            \
        (O).at[2][3] = xuniq(det) * -((M).at[0][0] * xuniq(a1312) -           \
                                      (M).at[0][1] * xuniq(a0312) +           \
                                      (M).at[0][3] * xuniq(a0112));           \
        (O).at[3][0] = xuniq(det) * -((M).at[1][0] * xuniq(a1223) -           \
                                      (M).at[1][1] * xuniq(a0223) +           \
                                      (M).at[1][2] * xuniq(a0123));           \
        (O).at[3][1] = xuniq(det) * ((M).at[0][0] * xuniq(a1223) -            \
                                     (M).at[0][1] * xuniq(a0223) +            \
                                     (M).at[0][2] * xuniq(a0123));            \
        (O).at[3][2] = xuniq(det) * -((M).at[0][0] * xuniq(a1213) -           \
                                      (M).at[0][1] * xuniq(a0213) +           \
                                      (M).at[0][2] * xuniq(a0113));           \
        (O).at[3][3] = xuniq(det) * ((M).at[0][0] * xuniq(a1212) -            \
                                     (M).at[0][1] * xuniq(a0212) +            \
                                     (M).at[0][2] * xuniq(a0112));            \
    } while (0)

#define xmat4_invert_f32(M, O) _xmat4_invert(float, M, O)

#define xmat4_invert_f64(M, O) _xmat4_invert(double, M, O)

#endif
