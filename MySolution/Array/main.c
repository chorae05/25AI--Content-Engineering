#include <stdio.h>
#include <stdlib.h>

#define Key_a 97
#define Key_z 122

void swap(char* a, char* b)
{
    char c = *a;
    *a = *b;
    *b = c;
}

int main()
{
    char random[Key_z - Key_a + 1];
    char sort[Key_z - Key_a + 1];
    int i;

    // random 배열 초기화
    for (i = 0; i < Key_z - Key_a + 1; i++) {
        random[i] = 0;
    }

    // random 배열에 a~z 랜덤 배치
    for (i = 0; i < Key_z - Key_a + 1; i++) {
        while (1) {
            int rand_index = rand() % (Key_z - Key_a + 1);
            if (random[rand_index] == 0) {
                random[rand_index] = (char)(i + Key_a);
                break;
            }
        }
    }

    for (i = 0; i < Key_z - Key_a; i++) {
        for (int j = 0; j < Key_z - Key_a - i; j++) {
            if (random[j] > random[j + 1]) {
                swap(&random[j], &random[j + 1]); // ✅ swap만 이용
            }
        }
    }

    
    // swap 테스트
    swap(&random[0], &random[1]);

    
    printf("\tindex\trandom\tsort");
    for (i = 0; i < Key_z - Key_a + 1; i++) {
        printf("\n\t%d\t%c", i, random[i] );
    }

    return 0;
}
