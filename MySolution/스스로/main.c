#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

#define MAX 100  

typedef struct {
    char name[20]; 
    int age;
    int number;
    double tiyou;
    
} Player;

void setTextColor(int color) {
    printf("\033[%dm", color);  // 31=빨강, 32=초록, 33=노랑, 36=하늘색, 37=흰색 등
}

void resetTextColor() {
    printf("\033[0m");  // 색상 초기화 (기본색으로 복귀)
}


int main() {
    char line[256];  // 파일에서 한 줄씩 읽을 공간
    int count = 0;  //사용자가 입력한 선수 수를 세는 변수  파일에 쓰기 전 단계
    int loadcount = 0;      //파일에서 읽은 선수 수를 세는 변수   파일을 읽은 후 단계

    Player player[MAX];

  

    //setTextColor(32);
    printf("몇 명의 선수를 입력받을까요? :");
    //setTextColor(0);
    scanf_s("%d", &count);
    

    if (count < 1 || count > MAX) {
        printf("잘못입력했습니다");
        return 1;
    }

    for (int i = 0; i < count; i++) {
        printf("%d 번쨰 선수다\n", i + 1);

        printf("이름: ");
        scanf_s("%s", player[i].name, (rsize_t)sizeof(player[i].name));

        printf("나이: ");
        scanf_s("%d", &player[i].age);

        printf("등번호: ");
        scanf_s("%d", &player[i].number);

        printf("타율: ");
        scanf_s("%lf", &player[i].tiyou);
    }

    FILE* file = NULL;
    errno_t err = fopen_s(&file, "players_lg.csv", "w");
    if (err != 0 || file == NULL) {
        printf("파일을 열 수 없습니다!\n");
        return -1;
    }
    //파일 or csv에 내가 입력한 것들이 작성되는 작업
    fprintf(file, "이름,나이,등번호,타율\n");

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s,%d,%d,%.3lf\n",
            player[i].name,
            player[i].age,
            player[i].number,
            player[i].tiyou);
    }

    fclose(file);

    printf("\n 파일 저장 완료! (%s)\n", "players_lg.csv");
    printf("%d 명의 정보를 CSV에 저장했습니다. -> %s\n", count, "players_lg.csv");
    printf("엔터를 누르면 csv를 다시 읽어옵니다.\n");
    _getch();


    FILE* fp = NULL;
    fopen_s(&fp, "players_lg.csv", "r");

    if (!fp) {
        printf("players_lg.csv 파일을 열 수 없습니다.\n");
        return 1; // 프로그램을 종료합니다.
    }

    //작성된 글이 나눠지는 과정
    //int loadcount = 0;      //파일에서 읽은 선수 수를 세는 변수   파일을 읽은 후 단계
    fgets(line, sizeof(line), fp);//윗줄은 고정

    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "가장 평균") != NULL) break; // 이 문장 만나면 종료

        char* t, * n;


        t = strtok_s(line, ",", &n);
        strcpy_s(player[loadcount].name, sizeof(player[loadcount].name), t);


        t = strtok_s(NULL, ",", &n);
        player[loadcount].age = atoi(t);


        t = strtok_s(NULL, ",", &n);
        player[loadcount].number = atoi(t);


        t = strtok_s(NULL, ",", &n);
        player[loadcount].tiyou = atof(t);

        loadcount++;
    }
        fclose(fp);

        printf("csv에서 %d명 로드됨 -> 표로 출력합니다.\n", loadcount);
        printf("---------------------------------------------------------\n");
        printf("name, age, number, tiyou\n");
        printf("---------------------------------------------------------\n");


        for (int i = 0; i < loadcount; i++) {
            Sleep(500);
            printf("%s, %d, %d, %.3lf\n", player[i].name, player[i].age, player[i].number, player[i].tiyou);
        }
        printf("출력을 모두 완료했습니다.\n 아무키나 누르면 종료됩니다\n");


        return 0;

    
}