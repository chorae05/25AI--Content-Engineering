#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int num;
    int* pArr;
    int average = 0;

    printf("몇개의 과목을 저장할 것인가 자네 : ");
    scanf_s("%d", &num);

    // 0개 넣으면 프로그램 종료해.
    if (num == 0)
    {
        return 0;
    }

    // 과목 개수 만큼 메모리 동적할당을 합니다.
    pArr = (int*)malloc(sizeof(int) * num);

    // 동적할당 실패
    if (pArr == NULL)
    {
        printf("malloc error");
        exit(1);
    }

    // 점수를 받고 저장합니다.
    for (int i = 0; i < num; ++i)
    {
        printf("[%d/%d]점수 입력 : ", i + 1, num);
        scanf_s("%d", &pArr[i]);
    }

    // 출력
    for (int i = 0; i < num; ++i)
    {
        printf("%d번째 과목의 점수 : %d\n", i + 1, pArr[i]);
    }

    // 평균 출력
    for (int i = 0; i < num; ++i)
    {
        average += pArr[i];
    }

    // 정수로 나누어 지기 때문에 소수점 이하는 버려집니다.
    average /= num;
    printf("평균 : %d\n", average);

    // 동적할당 해제
    free(pArr);

    system("pause");
    return 0;
}

/*
#include <stdio.h>

int main()
{
    int chess[10][10];
    int i, j;

    for (j = 0; j < 10; j++)
    {
        for (i = 0; i < 10; i++)
        {
            chess[i][j] = (i + j) % 2; // 0과 1을 번갈아 넣기
            printf("%d", chess[i][j]);
            //printf("%d", *(*(chess+i)+j));
        }
        printf("\n");
    }

    return 0;
}
*/