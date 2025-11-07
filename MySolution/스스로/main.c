#include <stdio.h>   
#include <stdlib.h>  
#include <string.h> 
#include <conio.h>   

typedef struct {
    char name[20];
    int atk;
    int def;
    int hp;
    char winner[250];
} Student;

// 싸움에서 이긴 사람의 이름을 저장
#define MAX 100

int main(void) {
    Student s[10];
    char name[20];
    char hakbun[20];
    int count = 0;
    char line[265];
    char winner[64];

    printf("학번: ");
    scanf_s("%s", hakbun, (rsize_t)sizeof(hakbun));

    printf("이름: ");
    scanf_s("%s", name, (rsize_t)sizeof(name));

    FILE* file = NULL;
    fopen_s(&file, "students.csv", "r");

    if (!file) {
        printf("파일을 열 수 없습니다!\n");
        return -1;
    }


    // 한 줄씩 읽기
    while (fgets(line, sizeof(line), file) && count < 10) {

        line[strcspn(line, "\n")] = 0;

        char* token;
        char* next;

        token = strtok_s(line, ",", &next);
        strcpy_s(s[count].name, sizeof(s[count].name), token);

        token = strtok_s(NULL, ",", &next);
        s[count].atk = atoi(token);

        token = strtok_s(NULL, ",", &next);
        s[count].def = atoi(token);

        token = strtok_s(NULL, ",", &next);
        s[count].hp = atoi(token);

        count++;
    }

    fclose(file);

    int fightmax = 0;
    int i = 1;
    for (i; i < count; i++)
        if (s[i].atk > s[fightmax].atk)
            fightmax = i;

    int i3 = 3, i8 = 8;

    
    /*(5) 다섯 번째 줄

3번째 사람과 8번째 사람이 싸운다고 가정하고, 승자 이름 또는 무승부를 출력한다.
(세 가지 중 하나)
ex>

3번째 vs 8번째 승자: 3번째사람이름
3번째 vs 8번째 승자: 8번째사람이름
3번째 vs 8번째: 무승부

*/



    FILE* T = NULL;
    fopen_s(&T, "Test.txt", "w");

    if (!T) {
        printf("Test. txt 파일을 열 수 없습니다!\n");
        return -1;
    }

    fprintf(T, "학번 : %s  이름 : %s\n", hakbun, name);
    fprintf(T, "가장 공격력이 높은 사람: %s (ATK=%d)\n", s[fightmax].name, s[fightmax].atk);
    fprintf(T, "3번째 : %s Hp=%d\n", s[i3].name, s[i3].hp);
    fprintf(T, "8번째 : %s Hp=%d\n", s[i8].name, s[i8].hp);


    fclose(T);
    printf("Test.txt 파일이 정상적으로 생성되었습니다!\n");

    return 0;

}