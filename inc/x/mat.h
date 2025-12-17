#ifndef X_MAT_H
#define X_MAT_H

#include <x/meta.h>

#define xmat(R, C, T)                                                         \
    union {                                                                   \
        T at[R][C];                                                           \
        T nth[(R) * (C)];                                                     \
    }

typedef xmat(3, 3, float) xmat3f32_t;
typedef xmat(3, 3, double) xmat3f64_t;
typedef xmat(4, 4, float) xmat4f32_t;
typedef xmat(4, 4, double) xmat4f64_t;

#define XMAT3_IDENTITY {[0] = 1, [4] = 1, [8] = 1}

#define XMAT4_IDENTITY {[0] = 1, [5] = 1, [10] = 1, [15] = 1}

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

#define xperspective()

#define xlook_at()

#endif
