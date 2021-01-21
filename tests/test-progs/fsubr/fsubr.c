#include <stdio.h>

float fsubr(float in1, float in2) {
    float ret = 0.0;
    asm ("fsubr %2, %0" : "=&t" (ret) : "%0" (in1), "u" (in2));
    return ret;
}

float fsub(float in1, float in2) {
    float ret = 0.0;
    asm ("fsub %2, %0" : "=&t" (ret) : "%0" (in1), "u" (in2));
    return ret;
}

int main() {
    printf("fsub (2.0, 1.8) = %f\n", fsub(2.0, 1.8));
    printf("fsubr(2.0, 1.8) = %f\n", fsubr(2.0, 1.8));
    return 0;
}
