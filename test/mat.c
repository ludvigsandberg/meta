#include <x/mat.h>
#include <assert.h>

int main(int argc, char **argv) {
    xmat3f32_t m3;
    xmat_identity_f32(m3);
    assert(m3.at[0][0] == 1.0f);
    assert(m3.at[1][1] == 1.0f);
    assert(m3.at[2][2] == 1.0f);
    assert(m3.at[0][1] == 0.0f);

    xmat4f32_t m4;
    xmat_identity_f32(m4);
    assert(m4.at[0][0] == 1.0f);
    assert(m4.at[1][1] == 1.0f);
    assert(m4.at[2][2] == 1.0f);
    assert(m4.at[3][3] == 1.0f);
    assert(m4.at[0][1] == 0.0f);

    /*xmat3f32_t a = {{1, 2, 3, 4, 5, 6, 7, 8, 9}};
    xmat3f32_t b = {{9, 8, 7, 6, 5, 4, 3, 2, 1}};
    xmat3f32_t result;
    xmat3_mul(a, b, result);
    assert(result.at[0][0] == 30.0f);
    assert(result.at[0][1] == 24.0f);
    assert(result.at[0][2] == 18.0f);*/

    xmat4f32_t m4a;
    xmat_identity_f32(m4a);
    xmat4f32_t m4b;
    xmat_identity_f32(m4b);
    xmat4f32_t m4result;
    xmat4_mul(m4a, m4b, m4result);
    assert(m4result.at[0][0] == 1.0f);
    assert(m4result.at[1][1] == 1.0f);
    assert(m4result.at[2][2] == 1.0f);
    assert(m4result.at[3][3] == 1.0f);

    float aspect_ratio = 1.f;
    float fov          = 70.f;
    xmat4f32_t projection;
    xperspective_f32(aspect_ratio, fov, 0.1f, 10000.f, projection);

    xvec3f32_t pos    = {{0.f, 1.f, 0.f}};
    xvec3f32_t target = {{0.f, 0.f, 0.f}};
    xvec3f32_t up     = {{0.f, 1.f, 0.f}};
    xmat4f32_t view;
    xlook_at_f32(pos, target, up, view);

    return 0;
}
