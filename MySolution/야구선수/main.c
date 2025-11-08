#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

#define ESC         "\x1b"  
#define CSI         ESC "["
#define CLR_RESET   CSI "0m"    //원상복구
#define FG_RED      CSI "31m"   //빨간색
#define FG_GREEN    CSI "32m"   //초록색
#define FG_YELLOW   CSI "33m"   //노란색
#define FG_BLUE     CSI "34m"   //파랑색
#define FG_MAGENTA  CSI "35m"   //보라색
#define FG_CYAN     CSI "36m"   //하늘색
#define FG_WHITE    CSI "37m"   //흰색
#define BOLD        CSI "1m"    //굵게    굵게 + 다른색  ex) BOLD FG_RED (굵은 빨간색)

#define MAX_PLAYERS 128

typedef struct {
    char name[64];
    int age;
    int number;
    double avg;
} Player;

int main(void)
{
    // ANSI 색상 출력 활성화
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        if (GetConsoleMode(hOut, &mode)) {
            mode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
            SetConsoleMode(hOut, mode);
        }
    }

    const char* CSV_PATH = "players_lg.csv";
    Player players[MAX_PLAYERS];
    int n = 0;

    printf(FG_BLUE BOLD "=== LG 선수 상태 CSV 저장/읽기 실습 ===\n" CLR_RESET);
    printf("이름/나이/등번호/타율을 입력 받아 CSV로 저장하고, 다시 읽어서 표로 출력합니다.\n");
    printf("문자열 입력은 공백 없는 이름을 권장합니다.\n\n");

    printf(FG_CYAN BOLD "[입력 단계]" CLR_RESET " 저장할 선수 수를 입력하세요 (최대 %d): ", MAX_PLAYERS);
    if (scanf_s("%d", &n) != 1 || n < 1 || n > MAX_PLAYERS) {
        printf(FG_RED "잘못된 개수입니다. 프로그램을 종료합니다.\n" CLR_RESET);
        return 1;
    }

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}

    for (int i = 0; i < n; ++i) {
        printf(FG_YELLOW "\n[%d/%d] 선수 정보 입력\n" CLR_RESET, i + 1, n);

        printf("  이름(공백 없이): ");
        if (scanf_s("%63s", players[i].name, (rsize_t)sizeof(players[i].name)) != 1) {
            printf(FG_RED "  입력 오류(이름)\n" CLR_RESET);
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            --i;
            continue;
        }

        printf("  나이(양의 정수): ");
        if (scanf_s("%d", &players[i].age) != 1 || players[i].age <= 0) {
            printf(FG_RED "  입력 오류(나이)\n" CLR_RESET);
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            --i;
            continue;
        }

        printf("  등번호(양의 정수): ");
        if (scanf_s("%d", &players[i].number) != 1 || players[i].number <= 0) {
            printf(FG_RED "  입력 오류(등번호)\n" CLR_RESET);
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            --i;
            continue;
        }

        printf("  타율(0.000 ~ 0.999, 소수3자리 권장): ");
        if (scanf_s("%lf", &players[i].avg) != 1 || players[i].avg < 0.0 || players[i].avg >= 1.0) {
            printf(FG_RED "  입력 오류(타율)\n" CLR_RESET);
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            --i;
            continue;
        }

        while ((ch = getchar()) != '\n' && ch != EOF) {}
    }

    FILE* fp = NULL;
    errno_t err = fopen_s(&fp, CSV_PATH, "w");
    if (err != 0 || fp == NULL) {
        printf(FG_RED "파일 열기 실패(쓰기): %s (errno=%d)\n" CLR_RESET, CSV_PATH, err);
        return 1;
    }

    fprintf(fp, "name,age,number,avg\n");
    for (int i = 0; i < n; ++i) {
        fprintf(fp, "%s,%d,%d,%.3f\n",
            players[i].name, players[i].age, players[i].number, players[i].avg);
    }
    fclose(fp);

    printf(FG_GREEN "\n[저장 완료] %d명 정보를 CSV로 기록했습니다 → %s\n" CLR_RESET, n, CSV_PATH);
    printf("엔터를 누르면 CSV를 다시 읽어옵니다...");
    getchar();

    FILE* fp2 = NULL;
    err = fopen_s(&fp2, CSV_PATH, "r");
    if (err != 0 || fp2 == NULL) {
        printf(FG_RED "파일 열기 실패(읽기): %s (errno=%d)\n" CLR_RESET, CSV_PATH, err);
        return 1;
    }

    Player load[MAX_PLAYERS];
    int count = 0;
    char line[512];
    int line_no = 0;

    while (fgets(line, sizeof(line), fp2)) {
        ++line_no;
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) line[--len] = '\0';
        if (line_no == 1) continue;

        char* ctx = NULL;
        char* tok = strtok_s(line, ",", &ctx);
        if (!tok) continue;
        strcpy_s(load[count].name, sizeof(load[count].name), tok);

        tok = strtok_s(NULL, ",", &ctx);
        if (!tok) continue;
        load[count].age = atoi(tok);

        tok = strtok_s(NULL, ",", &ctx);
        if (!tok) continue;
        load[count].number = atoi(tok);

        tok = strtok_s(NULL, ",", &ctx);
        if (!tok) continue;
        load[count].avg = atof(tok);

        count++;
    }
    fclose(fp2);

    printf(FG_MAGENTA BOLD "\n[로드/출력 단계] CSV에서 %d명 로드됨 → 표로 출력합니다.\n" CLR_RESET, count);
    printf(BOLD FG_WHITE "---------------------------------------------------------\n" CLR_RESET);
    printf(BOLD FG_WHITE "%-16s %-6s %-8s %-8s\n" CLR_RESET, "NAME", "AGE", "NUMBER", "AVG");
    printf(BOLD FG_WHITE "---------------------------------------------------------\n" CLR_RESET);

    for (int i = 0; i < count; ++i) {
        Sleep(50);
        printf("%-16s %-6d %-8d %-8.3f\n",
            load[i].name, load[i].age, load[i].number, load[i].avg);
    }
    printf(BOLD FG_WHITE "---------------------------------------------------------\n" CLR_RESET);

    printf(FG_GREEN "\n출력을 모두 완료했습니다. " CLR_RESET);
    printf("아무 키나 누르면 종료합니다...");
    _getch();

    printf("\n" FG_CYAN "실습 종료. 수고했다. (파일: %s)\n" CLR_RESET, CSV_PATH);
    return 0;
}
