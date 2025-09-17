#include <stdio.h>
#include <math.h>
#include <Windows.h>

#define PI 3.14159265358979

void moveCursor(int x, int y)
{
    printf("\x1b[%d;%dH", y, x);
}

//매개변수 isExploded
//0: 폭발전 폭탄
//1: 폭발함
void printBomb(int isExploded)
{
    //폭탄은 항상 7칸
    if (isExploded)
    {
        printf("\x1b[A^^^^^^^");
        printf("\x1b[B\x1b[7D!!BAM!!");
        printf("\x1b[B\x1b[7D^^^^^^^");
    }
    else
        printf("(  b  )");
}

/* --- ANSI VT 켜기 & 커서 숨김/표시 (ANSI 이스케이프 사용은 허용) --- */
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

/* 경로 좌표 */
typedef struct { int x, y; } P;

int main()
{
    enable_vt();
    hide_cursor();

    /* 1) 불꽃 시작 위치: (14, 9) */
    const int startX = 15;
    const int startY = 10;

    /* 2) 반시계 사각 나선(첫 방향: Y축 +방향(아래)) 경로 생성 */
    P path[256];
    int n = 0;

    int x = 0, y = 0;                        // 임시 원점에서 생성 후 평행이동
    int dir = 0;                              // 0:아래, 1:오른쪽, 2:위, 3:왼쪽
    const int D[4][2] = { {0,1}, {1,0}, {0,-1}, {-1,0} };
    int len = 1;
    int R = 6;                                // 나선 레이어(길이). 필요하면 늘리면 됨.

    for (int r = 0; r < R; ++r) {
        for (int k = 0; k < 2; ++k) {
            for (int s = 0; s < len; ++s) {
                path[n].x = x;
                path[n].y = y;
                ++n;
                x += D[dir][0];
                y += D[dir][1];
            }
            dir = (dir + 1) % 4;             // 반시계 회전
        }
        ++len;
    }

    /* 마지막 점(바깥 끝)을 (14,9)에 오도록 전체 평행이동 */
    int dx = startX - path[n - 1].x;
    int dy = startY - path[n - 1].y;
    for (int i = 0; i < n; ++i) { path[i].x += dx; path[i].y += dy; }

    /* 3) 심지 시작(path[0]) 왼쪽에 폭탄 배치 (폭탄 7칸이므로 x-7에서 출력) */
    moveCursor(path[0].x - 7, path[0].y);
    printBomb(0); // "(  b  )"  ← 이미지처럼 바로 오른쪽에 심지 시작

    /* 4) 심지 전체를 '#'로 먼저 그리기 (이미지의 초기 화면) */
    for (int i = 0; i < n; ++i) {
        moveCursor(path[i].x, path[i].y);
        printf("#");
    }

    /* 5) 불꽃 '*'을 시작 위치(14,9)에 찍기 */
    moveCursor(startX, startY);
    printf("*");

    /* --- 여기까지가 “처음 화면” --- */

    /* 6) 200ms마다 심지를 태우며 이동 후 폭발 */
    for (int i = n - 1; i > 0; --i) {
        Sleep(200);
        moveCursor(path[i].x, path[i].y); printf(" ");   // 현재 자리 태움
        moveCursor(path[i - 1].x, path[i - 1].y); printf("*"); // 다음 칸에 불꽃
    }

    // 폭탄 도착 → 마지막 자리 정리 후 폭발
    Sleep(200);
    moveCursor(path[0].x, path[0].y); printf(" ");
    moveCursor(path[0].x, path[0].y);
    printBomb(1); // 터지는 연출

    show_cursor();
    moveCursor(10, 20); // 마무리 커서 위치
    return 0;
}
