#include <stdio.h>

void sum(const int* x, const int* y, int* result) {
    *result = *x + *y;
}

int main(void) {
    int a = 3;
    int b = 2;
    int c = 0;

    int* pa = &c;

    sum(&a, &b, &c);     // a, b, c�� �ּҸ� �ѱ�(������ ���)

    printf("%d", *pa);   // c�� ��(=5) ���
    return 0;
}