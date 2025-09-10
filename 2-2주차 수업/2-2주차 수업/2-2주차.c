
#include<stdio.h>
#include<time.h>
#include<windows.h>

void gotoXY(int x, int y)
{
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
/*
*****
*   *
*   *
*   *
*****
 y = 0 ; x 0 ~ 4
 x = 4 : y 0 ~ 4
 y = 4 ; x 4 ~ 0
 x = 0 l y 4 ~ 0
0 1 2 3 4
1       1
2		2
3		3
0 1 2 3 4
*/
int main() {
	int x = 0;
	int y = 0;
	while (1) {

		gotoXY(x, y);
		printf("*");
		Sleep(100);

		if (x == 0 && y == 1) {
			break;
		}
		else if (y == 0 && x <= 3) {
			x++;
		}
		else if (x == 4 && y <= 3) {
			y++;
		}
		else if (y == 4 && x <= 4 && x >= 1) {
			x--;
		}
		else if (x == 0 && y <= 4) {
			y--;
		}
	}
	gotoXY(6, 6);

	//system("cls");  // 숫자 10까지 다 출력하고 나면 화면을 싹 지움!
}

