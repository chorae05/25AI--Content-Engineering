#include <stdio.h>
#include <windows.h>

void gotoxy(int x, int y) {
    COORD pos = { x - 1, y - 1 };  // 0���� �����ϴ� ��ǥ��� ��ȯ
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int main() {
    int a = 1;
    int h = 1;
    while (a < 10)
    {
        printf("* ");
        a = a + 1;
    }
    while (h < 5)
    {
        printf("* \n");
        h = h + 1;
    }

    return 0;
}