#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct {
    char name[50];
    int atk;
    int def;
    int hp;
    int mp;
} Student;

int main() {
    Student stu[100];
    int count = 0;
    errno_t err;
    char line[256];
    char hakbun[20];
    char user_name[20];

    // 학번과 이름 입력
    printf("학번= ");
    scanf_s("%s", hakbun, (rsize_t)sizeof(hakbun));
    printf("이름= ");
    scanf_s("%s", user_name, (rsize_t)sizeof(user_name));


    // (2) CSV 파일 열기
    FILE* fp1 = NULL;
    err = fopen_s(&fp1, "students.csv", "r");
    if (err != 0 || fp1 == NULL) {
        printf("students.csv 파일 열기 실패(쓰기): %s (errno=%d)\n", "students.csv", err);
        return -1;
    }
    printf("파일 열기 성공!!");

    fgets(line, sizeof(line), fp1);

    //  한 줄씩 읽어서 구조체 배열에 저장
    while (fgets(line, sizeof(line), fp1) && count < 100) {
        char* next;
        char* token = strtok_s(line, ",", &next);
        if (token == NULL) continue;
        strcpy_s(stu[count].name, sizeof(stu[count].name), token);

        token = strtok_s(NULL, ",", &next);
        stu[count].atk = atoi(token);

        token = strtok_s(NULL, ",", &next);
        stu[count].def = atoi(token);

        token = strtok_s(NULL, ",", &next);
        stu[count].hp = atoi(token);

        token = strtok_s(NULL, ",", &next);
        stu[count].mp = atoi(token);

        count++;
    }
    fclose(fp1);

    //  Test.txt 파일 생성     
    // 
    FILE* fp2 = NULL;
    err = fopen_s(&fp2, "Test.txt", "w");
    if (err != 0 || fp2 == NULL) {
        printf(" test. txt 파일 열기 실패(쓰기): %s (errno=%d)\n", "Test.txt", err);
        return 1;
    }
    printf("테스트 파일 열기 성공");


    fprintf(fp2, "%s : %s\n", hakbun, user_name);

    //  7번째, 8번째 이름
    if (count >= 8)
        fprintf(fp2, "%s %s\n", stu[6].name, stu[7].name);
    else
        fprintf(fp2, "데이터가 부족합니다.\n");

    //  Mother vs Horo (찾아서 싸움)
    int idxMother = -1, idxHoro = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(stu[i].name, "Mother") == 0)
            idxMother = i;
        if (strcmp(stu[i].name, "Horo") == 0)
            idxHoro = i;
    }

    if (idxMother != -1 && idxHoro != -1) {
        int m_hp = stu[idxMother].hp;
        int h_hp = stu[idxHoro].hp;
        int m_mp = stu[idxMother].mp;
        int h_mp = stu[idxHoro].mp;

        while (m_hp > 0 && h_hp > 0) {
            int dmg_m = 0, dmg_h = 0;

            // Mother의 공격
            if (m_mp >= stu[idxMother].atk) {
                m_mp -= stu[idxMother].atk;
                dmg_m = stu[idxMother].atk - stu[idxHoro].def;
                if (dmg_m < 0) dmg_m = 0;
            }
            else {
                m_mp += 10;
                dmg_m = (stu[idxMother].atk / 2) - stu[idxHoro].def;
                if (dmg_m < 0) dmg_m = 0;
            }

            // Horo의 공격
            if (h_mp >= stu[idxHoro].atk) {
                h_mp -= stu[idxHoro].atk;
                dmg_h = stu[idxHoro].atk - stu[idxMother].def;
                if (dmg_h < 0) dmg_h = 0;
            }
            else {
                h_mp += 10;
                dmg_h = (stu[idxHoro].atk / 2) - stu[idxMother].def;
                if (dmg_h < 0) dmg_h = 0;
            }

            // 피해 적용
            m_hp -= dmg_h;
            h_hp -= dmg_m;

            // 양쪽 피해 0이면 즉시 무승부
            if (dmg_h == 0 && dmg_m == 0) {
                fprintf(fp2, "무승부\n");
                goto skipBattle;
            }

            // 둘 다 0 이하인 경우 무승부
            if (m_hp <= 0 && h_hp <= 0) {
                fprintf(fp2, "무승부\n");
                goto skipBattle;
            }
        }

        if (m_hp > 0 && h_hp <= 0)
            fprintf(fp2, "%s 승! , %s 패\n", stu[idxMother].name, stu[idxHoro].name);
        else if (h_hp > 0 && m_hp <= 0)
            fprintf(fp2, "%s 승! , %s 패\n", stu[idxHoro].name, stu[idxMother].name);
        else
            fprintf(fp2, "무승부\n");
    }
    else {
        fprintf(fp2, "Mother 또는 Horo를 찾을 수 없습니다.\n");
    }

skipBattle:

    // Mother와 싸워서 이길 수 있는 사람들
    if (idxMother != -1) {
        int motherIdx = idxMother;
        int found = 0;

        for (int i = 0; i < count; i++) {
            if (i == motherIdx) continue;

            int m_hp = stu[motherIdx].hp;
            int o_hp = stu[i].hp;
            int m_mp = stu[motherIdx].mp;
            int o_mp = stu[i].mp;

            while (m_hp > 0 && o_hp > 0) {
                int dmg_m = 0, dmg_o = 0;

                if (m_mp >= stu[motherIdx].atk) {
                    m_mp -= stu[motherIdx].atk;
                    dmg_m = stu[motherIdx].atk - stu[i].def;
                    if (dmg_m < 0) dmg_m = 0;
                }
                else {
                    m_mp += 10;
                    dmg_m = (stu[motherIdx].atk / 2) - stu[i].def;
                    if (dmg_m < 0) dmg_m = 0;
                }

                if (o_mp >= stu[i].atk) {
                    o_mp -= stu[i].atk;
                    dmg_o = stu[i].atk - stu[motherIdx].def;
                    if (dmg_o < 0) dmg_o = 0;
                }
                else {
                    o_mp += 10;
                    dmg_o = (stu[i].atk / 2) - stu[motherIdx].def;
                    if (dmg_o < 0) dmg_o = 0;
                }

                m_hp -= dmg_o;
                o_hp -= dmg_m;

                if (dmg_o == 0 && dmg_m == 0) break;
                if (m_hp <= 0 && o_hp <= 0) break;
            }

            if (o_hp > 0 && m_hp <= 0) {
                if (found == 0) fprintf(fp2, "%s", stu[i].name);
                else fprintf(fp2, ", %s", stu[i].name);
                found = 1;
            }
        }

        if (found)
            fprintf(fp2, " 승!\n");
        else
            fprintf(fp2, "%s 승!\n", stu[motherIdx].name);
    }

    // 9번째,10번째,11번째 이름 연속 출력
    if (count >= 11)
        fprintf(fp2, "%s%s%s\n", stu[8].name, stu[9].name, stu[10].name);
    else
        fprintf(fp2, "데이터가 부족합니다.\n");

    //  평균 atk, def 계산
    int total_atk = 0, total_def = 0;
    for (int i = 0; i < count; i++) {
        total_atk += stu[i].atk;
        total_def += stu[i].def;
    }
    int avg_atk = total_atk / count;
    int avg_def = total_def / count;

    fprintf(fp2, "Average atk: %d\n", avg_atk);
    fprintf(fp2, "Average def: %d\n", avg_def);

    // 마지막 줄
    fprintf(fp2, "교수님 시험문제 너무 쉽습니다. 담주에 더 어렵게 내주세요\n");

    fclose(fp2);
    printf("Test.txt 파일 생성 완료!\n");
    return 0;
}
