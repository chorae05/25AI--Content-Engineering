#include <stdio.h>

void sum(const int* x, const int* y, int* result) {
    *result = *x + *y;
}

int main(void) {
    int a = 3;
    int b = 2;
    int c = 0;

    int* pa = &c;

    sum(&a, &b, &c);     // a, b, c의 주소를 넘김(포인터 사용)

    printf("%d", *pa);   // c의 값(=5) 출력
    return 0;
}