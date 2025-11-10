#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct {
    char name[20];
    int atk;
    int def;
    int hp;
} Student;

int main() {
    Student s[10];
    char line[100];
    int count = 0;
    errno_t err;

    // CSV 파일 읽기
    FILE* fp1 = NULL;
    err = fopen_s(&fp1, "students.csv", "r");
    if (err != 0 || fp1 == NULL) {
        printf("students.csv 파일을 열 수 없습니다.\n");
        return 1;
    }

    fgets(line, sizeof(line), fp1);


    while (fgets(line, sizeof(line), fp1) != NULL && count < 10) {
        char* next;
        char* token = strtok_s(line, ",", &next);
        if (token == NULL) continue;
        strcpy_s(s[count].name, sizeof(s[count].name), token);

        token = strtok_s(NULL, ",", &next);
        s[count].atk = atoi(token);

        token = strtok_s(NULL, ",", &next);
        s[count].def = atoi(token);

        token = strtok_s(NULL, ",", &next);
        s[count].hp = atoi(token);

        count++;
    }
    fclose(fp1);

    // txt 파일로 출력
    FILE* fp2 = NULL;
    err = fopen_s(&fp2, "Test.txt", "w");
    if (err != 0 || fp2 == NULL) {
        printf("Test.txt 파일을 만들 수 없습니다.\n");
        return 1;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp2, "%s,%d,%d,%d\n", s[i].name, s[i].atk, s[i].def, s[i].hp);
    }
    fclose(fp2);

    // 이름 변경 후 콘솔창 출력
    char oldname[20];
    char newname[20];

    printf("바꾸고 싶은 이름 입력: ");
    scanf_s("%s", oldname, (rsize_t)sizeof(oldname));

    printf("새로운 이름 입력: ");
    scanf_s("%s", newname, (rsize_t)sizeof(newname));

    printf("\n=== 이름 변경 후 결과 ===\n");

    for (int i = 0; i < count; i++) {
        if (strcmp(s[i].name, oldname) == 0) {
            strcpy_s(s[i].name, sizeof(s[i].name), newname);
        }
        printf("%s,%d,%d,%d\n", s[i].name, s[i].atk, s[i].def, s[i].hp);
    }

    return 0;
}
