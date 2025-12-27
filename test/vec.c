#include <assert.h>

#include <x/vec.h>

int main(int argc, char **argv) {
    xvec3f32_t a = {{1.f, 2.f, 3.f}};
    xvec3f32_t b = {{4.f, 5.f, 6.f}};

    xvec3f32_t res;

    xvec_add(a, b, res);
    assert(res.xyz.x == 5.f && res.xyz.y == 7.f && res.xyz.z == 9.f);

    xvec_sub(a, b, res);
    assert(res.xyz.x == -3.f && res.xyz.y == -3.f && res.xyz.z == -3.f);

    xvec_mul(a, b, res);
    assert(res.xyz.x == 4.f && res.xyz.y == 10.f && res.xyz.z == 18.f);

    xvec_div(b, a, res);
    assert(res.xyz.x == 4.f && res.xyz.y == 2.5f && res.xyz.z == 2.f);

    float len_a;
    xvec_len_f32(a, len_a);
    float expected_len_a = sqrtf(1.f * 1.f + 2.f * 2.f + 3.f * 3.f);
    assert(len_a >= expected_len_a - 0.0001f &&
           len_a <= expected_len_a + 0.0001f);

    xvec3f32_t norm_a;
    xvec_norm_f32(a, norm_a);
    float len_norm_a;
    xvec_len_f32(norm_a, len_norm_a);
    assert(len_norm_a >= 0.9999f && len_norm_a <= 1.0001f);

    float dot_res;
    xvec_dot(a, b, dot_res);
    assert(dot_res == 32.f);

    xvec3f32_t cross_res;
    xvec3_cross(a, b, cross_res);
    assert(cross_res.xyz.x == -3.f && cross_res.xyz.y == 6.f &&
           cross_res.xyz.z == -3.f);

    xvec2f32_t v2a = {{1.f, 2.f}};
    xvec2f32_t v2b = {{3.f, 4.f}};
    xvec2f32_t v2res;
    xvec_add(v2a, v2b, v2res);
    assert(v2res.xy.x == 4.f && v2res.xy.y == 6.f);

    xvec4f32_t v4a = {{1.f, 2.f, 3.f, 4.f}};
    xvec4f32_t v4b = {{5.f, 6.f, 7.f, 8.f}};
    xvec4f32_t v4res;
    xvec_add(v4a, v4b, v4res);
    assert(v4res.xyzw.x == 6.f && v4res.xyzw.y == 8.f &&
           v4res.xyzw.z == 10.f && v4res.xyzw.w == 12.f);

    return 0;
}
