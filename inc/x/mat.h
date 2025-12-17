#ifndef X_MAT_H
#define X_MAT_H

#define xmat(R, C, T)                                                         \
    union {                                                                   \
        T nth[(R) * (C)];                                                     \
        T at[R][C];                                                           \
    }

typedef xmat(3, 3, float) xmat3f32;
typedef xmat(3, 3, double) xmat3f64;
typedef xmat(4, 4, float) xmat4f32;
typedef xmat(4, 4, double) xmat4f64;

// mul (row x k) by (k x col) into (row x col)
#define xmat_mul(R, K, C, A, B, O)                                            \
    do {                                                                      \
        for (int r = 0; r < (R); r++)                                         \
            for (int c = 0; c < (C); c++) {                                   \
                (O).at[r][c] = 0;                                             \
                                                                              \
                for (int k = 0; k < (K); k++) {                               \
                    (O).at[r][c] += (A).at[r][k] * (B).at[k][c];              \
                }                                                             \
            }                                                                 \
    } while (0)

#endif
