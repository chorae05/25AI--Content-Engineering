#include <stdio.h>
#include <math.h>
#include <Windows.h>

#define PI 3.14159265358979323846

// 커서 이동 함수
void moveCursor(int x, int y) {
    printf("\x1b[%d;%dH", y, x);
}

// 폭탄 출력 함수
void printBomb(int isExploded) {
    if (isExploded) {
        printf("\x1b[A^^^^^^^");
        printf("\x1b[B\x1b[7D!!BAM!!");
        printf("\x1b[B\x1b[7D^^^^^^^");
    }
    else {
        printf("(  b  )");
    }
}
