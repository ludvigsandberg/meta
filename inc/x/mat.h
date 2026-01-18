#ifndef X_MAT_H
#define X_MAT_H

#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include <x/meta.h>
#include <x/vec.h>

// column major matrix
#define xmat(R, C, T)                                                         \
    union {                                                                   \
        T at[C][R];                                                           \
        T nth[(R) * (C)];                                                     \
    }

typedef xmat(3, 3, float) xmat3f32_t;
typedef xmat(3, 3, double) xmat3f64_t;
typedef xmat(4, 4, float) xmat4f32_t;
typedef xmat(4, 4, double) xmat4f64_t;

#define xmat_rows(M) (sizeof((M).at[0]) / sizeof((M).at[0][0]))

#define xmat_cols(M) (sizeof((M).at) / sizeof((M).at[0]))

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
        for (int c = 0; c < (C); ++c)                                         \
            for (int r = 0; r < (R); ++r) {                                   \
                (O).at[c][r] = 0;                                             \
                for (int k = 0; k < (K); ++k) {                               \
                    (O).at[c][r] += (A).at[k][r] * (B).at[c][k];              \
                }                                                             \
            }                                                                 \
    } while (0)

#define xmat3_mul(A, B, O) xmat_mul(3, 3, 3, A, B, O)

#define xmat4_mul(A, B, O) xmat_mul(4, 4, 4, A, B, O)

#define _xmat4_translate(M, T, VEC, O)                                        \
    do {                                                                      \
        xmat(4, 4, T) xuniq2(m, _xmat4_translate);                            \
        _xmat_identity(xuniq2(m, _xmat4_translate), T);                       \
        (xuniq2(m, _xmat4_translate)).at[3][0] = (VEC).nth[0];                \
        (xuniq2(m, _xmat4_translate)).at[3][1] = (VEC).nth[1];                \
        (xuniq2(m, _xmat4_translate)).at[3][2] = (VEC).nth[2];                \
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
        (O).at[3][0] = -xuniq(dot);                                           \
                                                                              \
        xvec_dot(xuniq(local_up_norm), POS, xuniq(dot));                      \
        (O).at[3][1] = -xuniq(dot);                                           \
                                                                              \
        xvec_dot(xuniq(forward), POS, xuniq(dot));                            \
        (O).at[3][2] = xuniq(dot);                                            \
    } while (0)

// create view matrix
#define xlook_at_f32(POS, TARGET, UP, O) _xlook_at(float, POS, TARGET, UP, O)

// create view matrix
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

// create perspective projection matrix
#define xperspective_f32(ASPECT, FOV, NEAR, FAR, O)                           \
    _xperspective(float, tanf, ASPECT, FOV, NEAR, FAR, O)

// create perspective projection matrix
#define xperspective_f64(ASPECT, FOV, NEAR, FAR, O)                           \
    _xperspective(double, tan, ASPECT, FOV, NEAR, FAR, O)

#define _xmat4_invert_view(VIEW, T, O)                                        \
    do {                                                                      \
        for (int xuniq(i) = 0; xuniq(i) < 3; ++xuniq(i)) {                    \
            for (int xuniq(j) = 0; xuniq(j) < 3; ++xuniq(j)) {                \
                (O).at[xuniq(i)][xuniq(j)] = (VIEW).at[xuniq(j)][xuniq(i)];   \
            }                                                                 \
        }                                                                     \
                                                                              \
        for (int xuniq(i) = 0; xuniq(i) < 3; ++xuniq(i)) {                    \
            (O).at[3][xuniq(i)] = -((O).at[0][xuniq(i)] * (VIEW).at[3][0] +   \
                                    (O).at[1][xuniq(i)] * (VIEW).at[3][1] +   \
                                    (O).at[2][xuniq(i)] * (VIEW).at[3][2]);   \
        }                                                                     \
                                                                              \
        (O).at[0][3] = (O).at[1][3] = (O).at[2][3] = (T)0;                    \
        (O).at[3][3]                               = (T)1;                    \
    } while (0)

// invert view matrix
#define xmat4_invert_view_f32(VIEW, O) _xmat4_invert_view(VIEW, float, O)

// invert view matrix
#define xmat4_invert_view_f64(VIEW, O) _xmat4_invert_view(VIEW, double, O)

#endif
