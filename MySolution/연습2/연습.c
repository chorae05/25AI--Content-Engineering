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
        fprintf(file, "%s,%d,%d,%.3f\n", players[i].name, players[i].age, players[i].number, players[i].avg);
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
/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

#define ESC         "\x1b"
#define CSI         ESC "["
#define CLR_RESET   CSI "0m"
#define FG_RED      CSI "31m"
#define FG_GREEN    CSI "32m"
#define FG_YELLOW   CSI "33m"
#define FG_BLUE     CSI "34m"
#define FG_MAGENTA  CSI "35m"
#define FG_CYAN     CSI "36m"
#define FG_WHITE    CSI "37m"
#define BOLD        CSI "1m"

static void enable_ansi_if_possible(void)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD mode = 0;
    if (!GetConsoleMode(hOut, &mode)) return;
    mode |= 0x0004;
    SetConsoleMode(hOut, mode);
}

typedef struct {
    char name[64];
    int age;
    int number;
    double avg;
} Player;

#define MAX_PLAYERS 128

static void flush_stdin_line(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

static int write_players_to_csv_interactive(const char* csv_path)
{
    Player players[MAX_PLAYERS];
    int n = 0;

    printf(FG_CYAN BOLD "[입력 단계]" CLR_RESET " 저장할 선수 수를 입력하세요 (최대 %d): ", MAX_PLAYERS);
    if (scanf_s("%d", &n) != 1 || n < 1 || n > MAX_PLAYERS) {
        printf(FG_RED "잘못된 개수입니다. 프로그램을 종료합니다.\n" CLR_RESET);
        flush_stdin_line();
        return -1;
    }
    flush_stdin_line();

    for (int i = 0; i < n; ++i) {
        printf(FG_YELLOW "\n[%d/%d] 선수 정보 입력\n" CLR_RESET, i + 1, n);

        printf("  이름(공백 없이): ");
        if (scanf_s("%63s", players[i].name, (rsize_t)sizeof(players[i].name)) != 1) {
            printf(FG_RED "  입력 오류(이름)\n" CLR_RESET);
            flush_stdin_line();
            --i;
            continue;
        }

        printf("  나이(양의 정수): ");
        if (scanf_s("%d", &players[i].age) != 1 || players[i].age <= 0) {
            printf(FG_RED "  입력 오류(나이)\n" CLR_RESET);
            flush_stdin_line();
            --i;
            continue;
        }

        printf("  등번호(양의 정수): ");
        if (scanf_s("%d", &players[i].number) != 1 || players[i].number <= 0) {
            printf(FG_RED "  입력 오류(등번호)\n" CLR_RESET);
            flush_stdin_line();
            --i;
            continue;
        }

        printf("  타율(0.000 ~ 0.999, 소수3자리 권장): ");
        if (scanf_s("%lf", &players[i].avg) != 1 || players[i].avg < 0.0 || players[i].avg >= 1.0) {
            printf(FG_RED "  입력 오류(타율)\n" CLR_RESET);
            flush_stdin_line();
            --i;
            continue;
        }

        flush_stdin_line();
    }

    FILE* fp = NULL;
    errno_t err = fopen_s(&fp, csv_path, "w");
    if (err != 0 || fp == NULL) {
        printf(FG_RED "파일 열기 실패(쓰기): %s  (errno=%d)\n" CLR_RESET, csv_path, err);
        return -2;
    }

    fprintf(fp, "name,age,number,avg\n");
    for (int i = 0; i < n; ++i) {
        fprintf(fp, "%s,%d,%d,%.3f\n",
            players[i].name, players[i].age, players[i].number, players[i].avg);
    }
    fclose(fp);

    printf(FG_GREEN "\n[저장 완료] %d명 정보를 CSV로 기록했습니다 → %s\n" CLR_RESET, n, csv_path);
    printf("엔터를 누르면 CSV를 다시 읽어옵니다...");
    getchar();

    return n;
}

static int read_players_from_csv_and_print(const char* csv_path)
{
    Player players[MAX_PLAYERS];
    int count = 0;
    int bignumber = 0;

    FILE* fp = NULL;
    errno_t err = fopen_s(&fp, csv_path, "r");
    if (err != 0 || fp == NULL) {
        printf(FG_RED "파일 열기 실패(읽기): %s  (errno=%d)\n" CLR_RESET, csv_path, err);
        return -1;
    }

    char line[512];
    int line_no = 0;
    char namezz[512];
    
    while (fgets(line, sizeof(line), fp)) {
        ++line_no;
        size_t len = strlen(line);
        if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
                line[--len] = '\0';
            }
        }

        if (line_no == 1) continue;

        char* ctx = NULL;
        char* tok = NULL;

        tok = strtok_s(line, ",", &ctx);
        if (!tok) continue;
        char name_buf[64] = { 0 };
        strcpy_s(name_buf, sizeof(name_buf), tok);
        if (line_no == 3) strcpy_s(namezz, sizeof(namezz), name_buf);

        tok = strtok_s(NULL, ",", &ctx);
        if (!tok) continue;
        char* endp = NULL;
        long age_val = strtol(tok, &endp, 10);

        tok = strtok_s(NULL, ",", &ctx);
        if (!tok) continue;
        long number_val = strtol(tok, &endp, 10);
        if (number_val > bignumber) bignumber = number_val;

        tok = strtok_s(NULL, ",", &ctx);
        if (!tok) continue;
        double avg_val = strtod(tok, &endp);

        if (count < MAX_PLAYERS) {
            strcpy_s(players[count].name, sizeof(players[count].name), name_buf);
            players[count].age = (int)age_val;
            players[count].number = (int)number_val;
            players[count].avg = avg_val;
            ++count;
        }
    }

    fclose(fp);

    printf(FG_MAGENTA BOLD "\n[로드/출력 단계] CSV에서 %d명 로드됨 → 표로 출력합니다.\n" CLR_RESET, count);
    printf(BOLD FG_WHITE "---------------------------------------------------------\n" CLR_RESET);
    printf(BOLD FG_WHITE "%-16s %-6s %-8s %-8s\n" CLR_RESET, "NAME", "AGE", "NUMBER", "AVG");
    printf(BOLD FG_WHITE "---------------------------------------------------------\n" CLR_RESET);

    for (int i = 0; i < count; ++i) {
        Sleep(50);
        printf("%-16s %-6d %-8d %-8.3f\n",
            players[i].name, players[i].age, players[i].number, players[i].avg);
    }
    printf(BOLD FG_WHITE "---------------------------------------------------------\n" CLR_RESET);

    printf(FG_GREEN "\n출력을 모두 완료했습니다. " CLR_RESET);
    printf("아무 키나 누르면 종료합니다...");
    (void)_getch();

    return count;
}

int main(void)
{
    enable_ansi_if_possible();
    const char* CSV_PATH = "players_lg.csv";

    printf(FG_BLUE BOLD "=== LG 선수 상태 CSV 저장/읽기 실습 ===\n" CLR_RESET);
    printf("이름/나이/등번호/타율을 입력 받아 CSV로 저장하고, 다시 읽어서 표로 출력합니다.\n");
    printf("문자열 입력은 공백 없는 이름을 권장합니다. (공백 지원은 다음 시간에 fgets로 확장)\n\n");

    int wrote = write_players_to_csv_interactive(CSV_PATH);
    if (wrote < 0) {
        printf(FG_RED "입력/저장 단계에서 오류가 발생했습니다. 프로그램을 종료합니다.\n" CLR_RESET);
        return 1;
    }

    int loaded = read_players_from_csv_and_print(CSV_PATH);
    if (loaded < 0) {
        printf(FG_RED "읽기/출력 단계에서 오류가 발생했습니다.\n" CLR_RESET);
        return 2;
    }

    printf("\n" FG_CYAN "실습 종료. 수고했다. (파일: %s)\n" CLR_RESET, CSV_PATH);
    return 0;
}
*/