#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 선수 한 명의 정보를 저장할 구조체
typedef struct {
    char name[64];   // 이름
    int age;         // 나이
    int number;      // 등번호
    double avg;      // 타율
} Player;

#define MAX 100   // 최대 선수 수 (128 → 100으로 단순화)

// --------------------------------------------------
// 1. 사용자에게 선수 정보 입력받고 CSV로 저장
// --------------------------------------------------
int save_players(const char* filename)
{
    Player players[MAX];
    int count = 0;

    printf("몇 명의 선수 정보를 저장할까요? ");
    scanf_s("%d", &count); 

    // 잘못된 숫자 입력 시
    if (count <= 0 || count > MAX) {
        printf("잘못된 입력입니다!\n");
        return -1;
    }

    // 선수 정보 입력
    for (int i = 0; i < count; i++) {
        printf("\n[%d번째 선수]\n", i + 1);

        printf("이름: ");
        scanf_s("%s", players[i].name, (rsize_t)sizeof(players[i].name));

        printf("나이: ");
        scanf_s("%d", &players[i].age);

        printf("등번호: ");
        scanf_s("%d", &players[i].number);

        printf("타율(0.000~1.000): ");
        scanf_s("%lf", &players[i].avg);
    }

    // 파일 열기
    FILE* file = NULL;
    fopen_s(&file, filename, "w");

    if (file == NULL) {
        printf("파일을 열 수 없습니다!\n");
        return -1;
    }

    // 첫 줄(헤더)
    fprintf(file, "이름,나이,등번호,타율\n");

    // 선수 정보 저장
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%d,%d,%.3f\n",
            players[i].name, players[i].age, players[i].number, players[i].avg);
    }

    fclose(file);
    printf("\n 파일 저장 완료! (%s)\n", filename);

    return count;
}

// --------------------------------------------------
// 2. CSV에서 선수 정보 읽어와서 출력
// --------------------------------------------------
int load_players(const char* filename)
{
    Player players[MAX];
    int count = 0;
    char line[256];


    FILE* file = NULL;
    fopen_s(&file, filename, "r");

    if (file == NULL) {
        printf("파일을 찾을 수 없습니다!\n");
        return -1;
    }

    // 첫 줄(헤더) 건너뛰기
    fgets(line, sizeof(line), file);

    // 한 줄씩 읽기
    while (fgets(line, sizeof(line), file)) {
        // 문자열 자르기 (콤마 기준)
        char* token;
        char* next;

        token = strtok_s(line, ",", &next);
        strcpy_s(players[count].name, sizeof(players[count].name), token);

        token = strtok_s(NULL, ",", &next);
        players[count].age = atoi(token);

        token = strtok_s(NULL, ",", &next);
        players[count].number = atoi(token);

        token = strtok_s(NULL, ",", &next);
        players[count].avg = atof(token);

        count++;
    }

    fclose(file);

    // 출력
    printf("\n-------------------------\n");
    printf("이름           나이  번호   타율\n");
    printf("-------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-12s %-5d %-6d %-6.3f\n",
            players[i].name, players[i].age, players[i].number, players[i].avg);
    }

    printf("-------------------------\n");
    printf("총 %d명의 선수가 로드되었습니다.\n", count);

    return count;
}

// --------------------------------------------------
// 3. 메인 함수 (전체 실행 흐름)
// --------------------------------------------------
int main(void)
{
    const char* filename = "players_lg.csv";

    int saved = save_players(filename); // 저장
    if (saved <= 0) return 1;

    int loaded = load_players(filename); // 읽기
    if (loaded <= 0) return 2;

    return 0;
}
