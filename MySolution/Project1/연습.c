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
