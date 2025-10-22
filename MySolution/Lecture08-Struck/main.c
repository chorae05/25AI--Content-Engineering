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

    // 🔸 교수님 스타일 (상대경로)
    const char* countPath = "play_count.txt";
    const char* logPath = "log.txt";
    const char* scorePath = "score.txt";

    // 🔸 플레이 횟수 및 점수 불러오기
    int playCount = getPlayCount(countPath);
    playCount++; // 이번 게임 포함

    int totalCorrect = 0;
    getScore(scorePath, &totalCorrect);

    int totalPlayed = playCount - 1;

    printf("%d번째 게임입니다!!\n", playCount);


    struct Fish fishes[5] = {
        {"문제 1", "초래가 좋아하는 최애 교수님은?\n1) 이은석  2) 최명복  3) 조성국  4) 이석원\n> ",
         1, "은석짱 사랑해요", "넌 아직 은석 교수님의 매력에 빠지지 않았어"},

        {"문제 2", "초래가 받고 싶은 학점은?\n1) C+  2) B+  3) A+  4) F\n> ",
         3, "초래는 A+이지~~ ", "교수님을 협박해서 A+로 성적 위조하는 수밖에..."},

        {"문제 3", "초래가 좋아하는 계절은?\n1) 봄  2) 이은석의 계절  3) 겨울  4) 여름\n> ",
         2, "은석 교수님만 있으면 그게 초래가 좋아하는 계절이지", "넌 아직 은석짱의 매력에 빠지지 못했구나ㅋ"},

        {"문제 4", "은석 교수님이 초래를 부르는 별명은?\n1) 불만킹  2) 공주  3) 이쁜이  4) 꽁꽁주\n> ",
         1, "역시 불만킹이야", "교수님이 초래를 보는 눈빛을 다시 살펴보렴~^^"},

        {"문제 5", "은석 교수님이 초래한테 줄 학점은?\n1) A+  2) B+  3) C+  4) F\n> ",
         1, "전 받을 준비 됐어요!!! ", "더 공부할게요ㅠㅠ"}
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

        int correct = 0;
        int idx = rand() % 5;
        struct Fish caught = fishes[idx];

        printf("[%s]\n", caught.name);
        printf("문제 낸 사람: 초래 공주\n");
        printf("%s", caught.talk);

        int userAnswer;
        scanf_s("%d", &userAnswer);

        FILE* logFp = fopen(logPath, "a");
        if (logFp == NULL) {
            perror("log.txt 열기 실패");
            return 1;
        }

        fprintf(logFp, "문제 낸 사람: 초래 공주\n");

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

        totalCorrect += correct;
        totalPlayed = playCount;

        time_t now = time(NULL);
        struct tm* t = localtime(&now);
        fprintf(logFp, "[날짜] %04d-%02d-%02d %02d:%02d\n",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
        fprintf(logFp, "현재 누적 정답률: %d/%d (%.1f%%)\n----------------------\n",
            totalCorrect, totalPlayed, (double)totalCorrect / totalPlayed * 100);

        fclose(logFp);

        printf("\n현재 누적 정답률: %d/%d (%.1f%%)\n",
            totalCorrect, totalPlayed, (double)totalCorrect / totalPlayed * 100);

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
