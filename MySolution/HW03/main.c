#include <stdio.h>
#include <math.h>
#include <Windows.h>

#define PI 3.14159265358979323846

// Ŀ�� �̵� �Լ�
void moveCursor(int x, int y) {
    printf("\x1b[%d;%dH", y, x);
}

// ��ź ��� �Լ�
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

int main() {
    printf("\x1b[2J"); // ȭ�� �����

    int cx = 14, cy = 9; // �Ҳ� ���� ��ġ
    int x = cx, y = cy;
    int angle = 270; // ���� ����: �Ʒ�
    int step = 2;

    // ��ź ��ġ
    int bombX = 0, bombY = 3;
    moveCursor(bombX, bombY);
    printBomb(0);

    // === 1�ܰ�: ���� ��ü �׸��� ===
    int tx = x, ty = y;
    int tempAngle = angle;
    int tempStep = step;

    for (int layer = 0; layer < 6; layer++) {
        for (int repeat = 0; repeat < 2; repeat++) {
            int dx = (int)round(cos(tempAngle * PI / 180.0));
            int dy = -(int)round(sin(tempAngle * PI / 180.0));
            for (int i = 0; i < tempStep; i++) {
                tx += dx;
                ty += dy;
                moveCursor(tx, ty);
                printf("#");
            }
            tempAngle = (tempAngle + 90) % 360;
        }
        tempStep += 2;
    }

    x = cx; y = cy;
    moveCursor(x, y);
    printf("*");
    Sleep(200);

    for (int layer = 0; layer < 6; layer++) {
        for (int repeat = 0; repeat < 2; repeat++) {
            int dx = (int)round(cos(angle * PI / 180.0));
            int dy = -(int)round(sin(angle * PI / 180.0));
            for (int i = 0; i < step; i++) {
                moveCursor(x, y);
                printf(" ");

                x += dx;
                y += dy;

                moveCursor(x, y);
                printf("*");

                Sleep(200);
            }
            angle = (angle + 90) % 360;
        }
        step += 2;
    }

    moveCursor(bombX, bombY);
    printf(" ");
    printBomb(1);

    moveCursor(10, 20);
    return 0;
}
