#include <stdio.h>

typedef struct {
    char name[20];
    int age;
    int number;
    double avg;
} Player;

int main() {
    Player p;

    printf("이름 입력: ");
    scanf_s("%s", p.name, (rsize_t)sizeof(p.name));

    printf("나이 입력: ");
    scanf_s("%d", &p.age);

    printf("등번호 입력: ");
    scanf_s("%d", &p.number);

    printf("타율 입력: ");
    scanf_s("%lf", &p.avg);

    FILE* fp;
    fopen_s(&fp, "players_lg.csv", "w");
    fprintf(fp, "name,age,number,avg\n");
    fprintf(fp, "%s,%d,%d,%.3f\n", p.name, p.age, p.number, p.avg);
    fclose(fp);

    printf("저장 완료! (players_lg.csv)\n");
    return 0;
}



/*
#include <stdio.h>

// 선수 구조체 정의
typedef struct {
    char name[20];
    int age;
    int number;
    double avg;
} Player;

// --------------------------------------------------
// 선수 한 명의 정보를 입력받는 함수
// --------------------------------------------------
void input_player(Player* p)
{
    printf("이름 입력: ");
    scanf_s("%s", p->name, (rsize_t)sizeof(p->name));

    printf("나이 입력: ");
    scanf_s("%d", &p->age);

    printf("등번호 입력: ");
    scanf_s("%d", &p->number);

    printf("타율 입력: ");
    scanf_s("%lf", &p->avg);
}

// --------------------------------------------------
// 파일로 저장하는 함수
// --------------------------------------------------
void save_player(const Player* p)
{
    FILE* fp;
    fopen_s(&fp, "players_lg.csv", "w");  // 파일 이름 직접 지정

    if (fp == NULL) {
        printf("파일을 열 수 없습니다!\n");
        return;
    }

    fprintf(fp, "name,age,number,avg\n");
    fprintf(fp, "%s,%d,%d,%.3f\n", p->name, p->age, p->number, p->avg);

    fclose(fp);
    printf("저장 완료! (players_lg.csv)\n");
}

// --------------------------------------------------
// 프로그램의 전체 흐름 (main)
// --------------------------------------------------
int main(void)
{
    Player player;          // 1명의 선수
    input_player(&player);  // 입력 받기
    save_player(&player);   // 저장하기

    return 0;
}

*/