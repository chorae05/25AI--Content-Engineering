#include <stdio.h>
#include <math.h>
#include <Windows.h>

#define PI 3.14159265358979

void moveCursor(int x, int y)
{
    printf("\x1b[%d;%dH", y, x);
}

//�Ű����� isExploded
//0: ������ ��ź
//1: ������
void printBomb(int isExploded)
{
    //��ź�� �׻� 7ĭ
    if (isExploded)
    {
        printf("\x1b[A^^^^^^^");
        printf("\x1b[B\x1b[7D!!BAM!!");
        printf("\x1b[B\x1b[7D^^^^^^^");
    }
    else
        printf("(  b  )");
}

/* --- ANSI VT �ѱ� & Ŀ�� ����/ǥ�� (ANSI �̽������� ����� ���) --- */
static void enable_vt(void) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(hOut, &mode)) {
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, mode);
    }
}
static void hide_cursor(void) { printf("\x1b[?25l"); }
static void show_cursor(void) { printf("\x1b[?25h"); }

/* ��� ��ǥ */
typedef struct { int x, y; } P;

int main()
{
    enable_vt();
    hide_cursor();

    /* 1) �Ҳ� ���� ��ġ: (14, 9) */
    const int startX = 15;
    const int startY = 10;

    /* 2) �ݽð� �簢 ����(ù ����: Y�� +����(�Ʒ�)) ��� ���� */
    P path[256];
    int n = 0;

    int x = 0, y = 0;                        // �ӽ� �������� ���� �� �����̵�
    int dir = 0;                              // 0:�Ʒ�, 1:������, 2:��, 3:����
    const int D[4][2] = { {0,1}, {1,0}, {0,-1}, {-1,0} };
    int len = 1;
    int R = 6;                                // ���� ���̾�(����). �ʿ��ϸ� �ø��� ��.

    for (int r = 0; r < R; ++r) {
        for (int k = 0; k < 2; ++k) {
            for (int s = 0; s < len; ++s) {
                path[n].x = x;
                path[n].y = y;
                ++n;
                x += D[dir][0];
                y += D[dir][1];
            }
            dir = (dir + 1) % 4;             // �ݽð� ȸ��
        }
        ++len;
    }

    /* ������ ��(�ٱ� ��)�� (14,9)�� ������ ��ü �����̵� */
    int dx = startX - path[n - 1].x;
    int dy = startY - path[n - 1].y;
    for (int i = 0; i < n; ++i) { path[i].x += dx; path[i].y += dy; }

    /* 3) ���� ����(path[0]) ���ʿ� ��ź ��ġ (��ź 7ĭ�̹Ƿ� x-7���� ���) */
    moveCursor(path[0].x - 7, path[0].y);
    printBomb(0); // "(  b  )"  �� �̹���ó�� �ٷ� �����ʿ� ���� ����

    /* 4) ���� ��ü�� '#'�� ���� �׸��� (�̹����� �ʱ� ȭ��) */
    for (int i = 0; i < n; ++i) {
        moveCursor(path[i].x, path[i].y);
        printf("#");
    }

    /* 5) �Ҳ� '*'�� ���� ��ġ(14,9)�� ��� */
    moveCursor(startX, startY);
    printf("*");

    /* --- ��������� ��ó�� ȭ�顱 --- */

    /* 6) 200ms���� ������ �¿�� �̵� �� ���� */
    for (int i = n - 1; i > 0; --i) {
        Sleep(200);
        moveCursor(path[i].x, path[i].y); printf(" ");   // ���� �ڸ� �¿�
        moveCursor(path[i - 1].x, path[i - 1].y); printf("*"); // ���� ĭ�� �Ҳ�
    }

    // ��ź ���� �� ������ �ڸ� ���� �� ����
    Sleep(200);
    moveCursor(path[0].x, path[0].y); printf(" ");
    moveCursor(path[0].x, path[0].y);
    printBomb(1); // ������ ����

    show_cursor();
    moveCursor(10, 20); // ������ Ŀ�� ��ġ
    return 0;
}
