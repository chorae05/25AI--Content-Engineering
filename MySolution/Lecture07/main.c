#include <stdio.h>

int main()
{
    int chess[10][10];
    int i, j;

    for (j = 0; j < 10; j++)
    {
        for (i = 0; i < 10; i++)
        {
            chess[i][j] = (i + j) % 2; // 0�� 1�� ������ �ֱ�
            printf("%d", chess[i][j]);
            //printf("%d", *(*(chess+i)+j));
        }
        printf("\n");
    }

    return 0;
}
