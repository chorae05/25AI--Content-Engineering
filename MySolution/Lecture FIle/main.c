#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

struct Fish
{
    char* name;      // 물고기 이름
    char* talk;      // 문제
    int answer;      // 정답 번호
    char* talk_O;    // 맞았을 때 대사
    char* talk_X;    // 틀렸을 때 대사
};

// 🔹 플레이 횟수 불러오기
int getPlayCount(const char* path)
{
    FILE* fp = fopen(path, "r");
    if (fp == NULL) return 0;
    int count = 0;
    fscanf_s(fp, "%d", &count);
    fclose(fp);
    return count;
}

// 🔹 플레이 횟수 저장
void savePlayCount(const char* path, int count)
{
    FILE* fp = fopen(path, "w");
    if (fp == NULL) return;
    fprintf(fp, "%d", count);
    fclose(fp);
}

// 🔹 누적 정답률 불러오기
void getScore(const char* path, int* correct)
{
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        *correct = 0;
        return;
    }
    fscanf_s(fp, "%d", correct);
    fclose(fp);
}

// 🔹 누적 정답률 저장
void saveScore(const char* path, int correct)
{
    FILE* fp = fopen(path, "w");
    if (fp == NULL) return;
    fprintf(fp, "%d", correct);
    fclose(fp);
}

int main()
{
    srand((unsigned int)time(NULL));

    // 🔸 파일 경로 (MySolution 폴더에 고정)
    const char* countPath = "C:\\chorae\\25AI--Content-Engineering\\MySolution\\play_count.txt";
    const char* logPath = "C:\\chorae\\25AI--Content-Engineering\\MySolution\\log.txt";
    const char* scorePath = "C:\\chorae\\25AI--Content-Engineering\\MySolution\\score.txt";

    // 🔸 플레이 횟수 및 점수 불러오기
    int playCount = getPlayCount(countPath);
    playCount++; // 이번 게임 포함

    int totalCorrect = 0;
    getScore(scorePath, &totalCorrect);

    int totalPlayed = playCount - 1; // 이전까지 플레이한 횟수

    printf("🎣 %d번째 게임입니다!!\n", playCount);
    if (totalPlayed > 0)
        printf("현재까지 정답률: %d/%d (%.1f%%)\n", totalCorrect, totalPlayed,
            (double)totalCorrect / totalPlayed * 100);

    // 🔸 세자르의 물고기 퀴즈
    struct Fish fishes[5] = {
        {"금붕어", "세자르🐟: 내가 제일 자신 있는 스포츠는?\n1) 수영  2) 달리기  3) 농구  4) 씨름\n> ", 1, "이걸 맞추다니! 물살을 거슬러도 잘하겠군!", "내가 그럴 줄 알았다. 물 위에서도 달리긴 힘들지!"},
        {"복어", "세자르🐡: 내가 화나면 어떻게 될까?\n1) 부푼다  2) 울음소리 낸다  3) 사라진다  4) 웃는다\n> ", 1, "이걸 맞추다니! 복어 전문가네!", "내가 그럴 줄 알았다. 넌 나를 너무 얕봤지!"},
        {"상어", "세자르🦈: 상어가 싫어하는 노래는?\n1) Baby Shark  2) 상어송  3) 바다의 노래  4) 인어공주 OST\n> ", 1, "이걸 맞추다니! 네 귀도 상어급이구나!", "내가 그럴 줄 알았다. Baby Shark는 나한테 트라우마야."},
        {"고등어", "세자르🐠: 고등어가 제일 싫어하는 날씨는?\n1) 비 오는 날  2) 맑은 날  3) 구운 날  4) 눈 오는 날\n> ", 3, "이걸 맞추다니! 넌 이미 고등어급 개그센스야!", "내가 그럴 줄 알았다. 넌 아직 덜 익었어!"},
        {"니모", "세자르🐟: 내가 출연한 영화 제목은?\n1) 물고기와 친구들  2) 니모를 찾아서  3) 해저 탐험대  4) 수중왕국\n> ", 2, "이걸 맞추다니! 기억력 최고야!", "내가 그럴 줄 알았다. 디즈니는 좀 더 봐야겠네!"}
    };

    // 🔸 입력 반복
    char yn;
    while (1)
    {
        printf("\n찌를 던진다 (Y/N) > ");
        yn = _getch();
        printf("%c\n", yn);

        if (yn == 'Y' || yn == 'y' || yn == 'N' || yn == 'n')
            break;
        else
        {
            printf("잘못 입력했습니다. 다시 입력하세요.\n\n");
            Sleep(700);
        }
    }

    if (yn == 'Y' || yn == 'y')
    {
        printf("찌가 물 위에 떠 있다");
        for (int i = 0; i < 5; i++) {
            Sleep(400);
            printf(".");
        }
        printf("\n\033[32m입질이 왔다!!!\033[0m\n\n");

        int correct = 0; // 이번 문제 맞춘 여부
        int idx = rand() % 5;
        struct Fish caught = fishes[idx];

        printf("[%s 등장!]\n", caught.name);
        printf("문제 낸 사람: 세자르\n");
        printf("%s", caught.talk);

        int userAnswer;
        scanf_s("%d", &userAnswer);

        FILE* logFp = fopen(logPath, "a");
        if (logFp == NULL) {
            perror("log.txt 열기 실패");
            return 1;
        }

        fprintf(logFp, "문제 낸 사람: 세자르\n");

        if (userAnswer == caught.answer) {
            printf("%s\n", caught.talk_O);
            fprintf(logFp, "게임 %d - %s : O\n", playCount, caught.name);
            correct = 1;
        }
        else {
            printf("%s\n", caught.talk_X);
            fprintf(logFp, "게임 %d - %s : X\n", playCount, caught.name);
            correct = 0;
        }

        totalCorrect += correct; // 누적 정답 추가
        totalPlayed = playCount; // 전체 게임 횟수 = 플레이 횟수

        // 날짜 기록
        time_t now = time(NULL);
        struct tm* t = localtime(&now);
        fprintf(logFp, "[날짜] %04d-%02d-%02d %02d:%02d\n",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
        fprintf(logFp, "현재 누적 정답률: %d/%d (%.1f%%)\n----------------------\n",
            totalCorrect, totalPlayed, (double)totalCorrect / totalPlayed * 100);

        fclose(logFp);

        // 콘솔 출력
        printf("\n현재 누적 정답률: %d/%d (%.1f%%)\n",
            totalCorrect, totalPlayed, (double)totalCorrect / totalPlayed * 100);

        // 데이터 저장
        savePlayCount(countPath, playCount);
        saveScore(scorePath, totalCorrect);

        printf("\n결과가 log.txt에 저장되었습니다.\n");
    }
    else
    {
        printf("낚시를 포기했습니다.\n");
    }

    return 0;
}
