#include <x/vec.h>

int main(int argc, char **argv) {
    xvec3f32_t a = {{1.f, 2.f, 3.f}};
    a            = (xvec3f32_t){{4.f, 5.f, 6.f}};
    xvec3f32_t b = {{4.f, 5.f, 6.f}};
    b            = a;

    xvec3f32_t res;
    xvec_add(a, b, res);

    return 0;
}
