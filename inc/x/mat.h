#ifndef X_MAT_H
#define X_MAT_H

#include <x/meta.h>
#include <x/vec.h>

#define xmat(R, C, T)                                                         \
    union {                                                                   \
        T at[R][C];                                                           \
        T nth[(R) * (C)];                                                     \
    }

typedef xmat(3, 3, float) xmat3f32_t;
typedef xmat(3, 3, double) xmat3f64_t;
typedef xmat(4, 4, float) xmat4f32_t;
typedef xmat(4, 4, double) xmat4f64_t;

#define xmat3_identity(T) {.nth = {[0] = (T)1, [4] = (T)1, [8] = (T)1}}

#define xmat4_identity(T)                                                     \
    {.nth = {[0] = (T)1, [5] = (T)1, [10] = (T)1, [15] = (T)1}}

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
        (O)          = xmat4_identity(T);                                     \
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
        (O)          = {0};                                                   \
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

#endif
