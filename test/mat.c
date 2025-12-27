#include <x/mat.h>
#include <assert.h>

int main(int argc, char **argv) {
    xmat3f32_t m3 = xmat3_identity(float);
    assert(m3.at[0][0] == 1.0f);
    assert(m3.at[1][1] == 1.0f);
    assert(m3.at[2][2] == 1.0f);
    assert(m3.at[0][1] == 0.0f);

    xmat4f32_t m4 = xmat4_identity(float);
    assert(m4.at[0][0] == 1.0f);
    assert(m4.at[1][1] == 1.0f);
    assert(m4.at[2][2] == 1.0f);
    assert(m4.at[3][3] == 1.0f);
    assert(m4.at[0][1] == 0.0f);

    xmat3f32_t a = {{1, 2, 3, 4, 5, 6, 7, 8, 9}};
    xmat3f32_t b = {{9, 8, 7, 6, 5, 4, 3, 2, 1}};
    xmat3f32_t result;
    xmat3_mul(a, b, result);
    assert(result.at[0][0] == 30.0f);
    assert(result.at[0][1] == 24.0f);
    assert(result.at[0][2] == 18.0f);

    xmat4f32_t m4a = xmat4_identity(float);
    xmat4f32_t m4b = xmat4_identity(float);
    xmat4f32_t m4result;
    xmat4_mul(m4a, m4b, m4result);
    assert(m4result.at[0][0] == 1.0f);
    assert(m4result.at[1][1] == 1.0f);
    assert(m4result.at[2][2] == 1.0f);
    assert(m4result.at[3][3] == 1.0f);

    return 0;
}
