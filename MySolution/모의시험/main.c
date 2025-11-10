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
    errno_t err = fopen_s(&file, "students.csv", "r");
    if (err != 0 || file == NULL) {
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

    FILE* T = NULL;
    fopen_s(&T, "Test.txt", "w");

    if (!T) {
        printf("Test. txt 파일을 열 수 없습니다!\n");
        return -1;
    }

    fprintf(T, "%s : %s\n", hakbun, name);
    fprintf(T, "가장 공격력이 높은 사람: %s (ATK=%d)\n", s[fightmax].name, s[fightmax].atk);
    fprintf(T, "3번째: %s HP=%d\n", s[3].name, s[3].hp);
    fprintf(T, "8번째: %s HP=%d\n", s[8].name, s[8].hp);

    //fprintf(T, "3번째 vs 8번째 승자: %s\n", s[i3].atk > s[i8].atk ? s[i3].atk: s[i3].atk < s[i8].atk ? s[i8].atk:"무승부");

    if (s[3].atk > s[8].atk) {
        fprintf(T, "3번째 vs 8번째 승자: %s\n", s[3].name);
    }
    else if (s[3].atk < s[8].atk) {
        fprintf(T, "3번째 vs 8번째 승자: %s\n", s[8].name);
    }
    else {
        fprintf(T, "3번째 vs 8번째 승자: 무승부\n");
    }

    fprintf(T, "교수님 시험문제 너무 쉽습니다. 담주에 더 어렵게 내주세요\n");
    fclose(T);
    printf("Test.txt 파일이 정상적으로 생성되었습니다!\n");

    return 0;

}
