#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAX 100

typedef struct {
    char name[64];
    int atk, def, hp;
} Student;

int main(void)
{
    Student stu[10];
    char line[256];
    int count = 0;
    char id[20], user_name[64], winner[64];

    printf("학번을 입력하세요: ");
    scanf_s("%s", id, (rsize_t)sizeof(id));
    printf("이름을 입력하세요: ");
    scanf_s("%s", user_name, (rsize_t)sizeof(user_name));

    FILE* fp = NULL;
    fopen_s(&fp, "students.csv", "r");
    if (!fp) {
        printf("students.csv 파일을 열 수 없습니다.\n");
        return 1;
    }

    while (fgets(line, sizeof(line), fp) && count < 10) {
        char* t, * n;
        t = strtok_s(line, ",", &n);
        strcpy_s(stu[count].name, sizeof(stu[count].name), t);
        t = strtok_s(NULL, ",", &n); stu[count].atk = atoi(t);
        t = strtok_s(NULL, ",", &n); stu[count].def = atoi(t);
        t = strtok_s(NULL, ",", &n); stu[count].hp = atoi(t);
        count++;
    }
    fclose(fp);

    int max_idx = 0;
    for (int i = 1; i < count; i++)
        if (stu[i].atk > stu[max_idx].atk)
            max_idx = i;

    // ⚙️ 김철수(2번째, index=1) vs 오상우(7번째, index=6)
    int i3 = 3, i8 = 8;

    int damage3 = stu[i8].atk - stu[i3].def;
    int damage8 = stu[i3].atk - stu[i8].def;
    if (damage3 < 0) damage3 = 0;
    if (damage8 < 0) damage8 = 0;

    if (damage3 == 0 && damage8 == 0)
        strcpy_s(winner, sizeof(winner), "무승부");
    else {
        int hp3 = stu[i3].hp - damage3;
        int hp8 = stu[i8].hp - damage8;
        if (hp3 <= 0 && hp8 <= 0)
            strcpy_s(winner, sizeof(winner), "무승부");
        else if (hp3 <= 0)
            strcpy_s(winner, sizeof(winner), stu[i8].name);
        else if (hp8 <= 0)
            strcpy_s(winner, sizeof(winner), stu[i3].name);
        else if (hp3 > hp8)
            strcpy_s(winner, sizeof(winner), stu[i3].name);
        else if (hp8 > hp3)
            strcpy_s(winner, sizeof(winner), stu[i8].name);
        else
            strcpy_s(winner, sizeof(winner), "무승부");
    }

    FILE* out = NULL;
    fopen_s(&out, "Test.txt", "w");
    if (!out) {
        printf("Test.txt 파일을 쓸 수 없습니다.\n");
        return 1;
    }

    fprintf(out, "%s : %s\n", id, user_name);
    fprintf(out, "가장 공격력이 높은 사람: %s (ATK=%d)\n", stu[max_idx].name, stu[max_idx].atk);
    fprintf(out, "3번째: %s HP=%d\n", stu[i3].name, stu[i3].hp);
    fprintf(out, "8번째: %s HP=%d\n", stu[i8].name, stu[i8].hp);
    if (strcmp(winner, "무승부") == 0)
        fprintf(out, "3번째 vs 8번째: 무승부\n");
    else
        fprintf(out, "3번째 vs 8번째 승자: %s\n", winner);
    fprintf(out, "교수님 시험문제 너무 쉽습니다. 담주에 더 어렵게 내주세요");
    fclose(out);

    printf("결과가 Test.txt로 저장되었습니다!\n");
    return 0;
}
