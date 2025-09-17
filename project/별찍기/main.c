#include<stdio.h>
/*
int main() {
	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 5; x++) {
			if (y == 0 || y == 4 || x == 0 || x == 4)
				printf("*");
			else
				printf(" ");
		}
		printf("\n");
	}

	return 0;
}


int main(void) {
	puts("*****\n*   *\n*   *\n*   *\n*****");
	return 0;
}
*/
int main(void) {
	for (int i = 0; i < 5; i++) puts(i == 0 || i == 4 ? "*****" : "*   *");
	return 0;
}