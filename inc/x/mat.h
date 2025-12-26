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

#define _xlook_at(T, POS, TARGET, UP, O)                                      \
    do {                                                                      \
        xvec(3, T) diff;                                                      \
        xvec_sub(TARGET, POS, diff);                                          \
                                                                              \
        xvec(3, T) forward;                                                   \
        xvec_norm_f32(diff, forward);                                         \
                                                                              \
        xvec(3, T) right;                                                     \
        xvec3_cross(forward, UP, right);                                      \
        xvec(3, T) right_norm;                                                \
        xvec_norm_f32(right, right_norm);                                     \
                                                                              \
        xvec(3, T) local_up;                                                  \
        xvec3_cross(right_norm, forward, local_up);                           \
        xvec(3, T) local_up_norm;                                             \
        xvec_norm_f32(local_up, local_up_norm);                               \
                                                                              \
        (O).at[0][0] = right_norm.xyz.x;                                      \
        (O).at[1][0] = right_norm.xyz.y;                                      \
        (O).at[2][0] = right_norm.xyz.z;                                      \
        (O).at[0][1] = local_up_norm.xyz.x;                                   \
        (O).at[1][1] = local_up_norm.xyz.y;                                   \
        (O).at[2][1] = local_up_norm.xyz.z;                                   \
        (O).at[0][2] = -forward.xyz.x;                                        \
        (O).at[1][2] = -forward.xyz.y;                                        \
        (O).at[1][2] = -forward.xyz.z;                                        \
        (O).at[2][2] = right_norm.xyz.x;                                      \
        (O).at[3][0] = right_norm.xyz.x;                                      \
        (O).at[3][2] = right_norm.xyz.x;                                      \
    } while (0)

#define xperspective()

#endif

/*
mat4 vlk::look_at(vec3f pos, vec3f target, vec3f up) {
    vec3f diff     = target - pos;
    vec3f forward  = diff.normalize();
    vec3f right    = forward.cross(up).normalize();
    vec3f local_up = right.cross(forward).normalize();

    mat4 m{1.0f};
    m[0][0] = right.x();
    m[1][0] = right.y();
    m[2][0] = right.z();
    m[0][1] = local_up.x();
    m[1][1] = local_up.y();
    m[2][1] = local_up.z();
    m[0][2] = -forward.x();
    m[1][2] = -forward.y();
    m[2][2] = -forward.z();
    m[3][0] = -right.dot(pos);
    m[3][1] = -local_up.dot(pos);
    m[3][2] = forward.dot(pos);
    return m;
}

mat4 vlk::perspective(f32 aspect, f32 fov, f32 near, f32 far) {
    f32 half_tan = std::tan(fov / 2);

    mat4 m;
    m[0][0] = 1 / (half_tan * aspect);
    m[1][1] = 1 / half_tan;
    m[2][2] = -(far + near) / (far - near);
    m[2][3] = -1;
    m[3][2] = -(2 * far * near) / (far - near);
    return m;
}

void quaternion::set(const vec3f &axis, f32 angle) {
    f32 half_angle = 0.5f * angle;
    f32 s          = std::sin(half_angle);

    vec.x() = s * axis.x();
    vec.y() = s * axis.y();
    vec.z() = s * axis.z();
    vec.w() = std::cos(half_angle);
}
*/
