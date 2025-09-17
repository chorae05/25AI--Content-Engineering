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
    } else {
        printf("(  b  )");
    }
}

int main() {
    printf("\x1b[2J"); // 화면 지우기

    int cx = 14, cy = 9; // 불꽃 시작 위치
    moveCursor(cx, cy);
    printf("*");

    // 1) 경로 저장 배열
    int pathX[1024], pathY[1024];
    int pathLen = 0;

    // 2) 심지 경로 생성 + 출력
    int x = cx, y = cy;
    int angle = 270; // 시작 방향: 아래
    int step = 2;    // 처음 길이



    for (int layer = 0; layer < 6; layer++) {   // 나선 바퀴 수
        for (int repeat = 0; repeat < 2; repeat++) {
            int dx = (int)round(cos(angle * PI / 180.0));
            int dy = -(int)round(sin(angle * PI / 180.0)); // y축 반전 보정

            int thisStep = step;
            if (layer == 5 && repeat == 1) {
                thisStep += 15;   // 원하는 만큼 추가 길이
            }


            for (int i = 0; i < step; i++) {
                x += dx;
                y += dy;
                moveCursor(x, y);
                printf("#");


                // 좌표 저장
                pathX[pathLen] = x;
                pathY[pathLen] = y;
                pathLen++;
            }
            angle = (angle + 90) % 360; // 반시계로 90도 회전
        }
        step += 2; // 두 번 돌 때마다 길이 증가
    }


    // 3) 폭탄 출력 (심지 끝에)
    int bombX = pathX[pathLen - 1];
    int bombY = pathY[pathLen - 1];
    moveCursor(bombX, bombY);
    printBomb(0);

    // 4) 불꽃 애니메이션
    int fx = cx, fy = cy; // 시작 불꽃 위치
    for (int i = 0; i < pathLen; i++) {
        Sleep(200);

        // 다음 위치 미리 확인
        int nx = pathX[i];
        int ny = pathY[i];

        if (nx == bombX && ny == bombY) {
            // (  b  ) 지우기
            moveCursor(bombX, bombY);
            printf("       ");
            // 폭발 출력
            moveCursor(bombX, bombY);
            printBomb(1);
            return 0;
        }

        // 그 외에는 일반 이동
        moveCursor(fx, fy);  // 이전 불꽃 지우기
        printf(" ");

        moveCursor(nx, ny);  // 심지 지우기
        printf(" ");

        moveCursor(nx, ny);  // 불꽃 이동
        printf("*");

        fx = nx;
        fy = ny;
    }

    moveCursor(1, 25);
    return 0;

}
