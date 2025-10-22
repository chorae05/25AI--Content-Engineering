#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE* fp = NULL;
    const char* filename = "C:\\chorae\\25AI--Content-Engineering\\MySolution\\Lecture File\\log.txt";

    fopen_s(&fp, filename, "a");
    if (fp == NULL) {
        perror("파일 열기 실패");
        return 1;
    }

    fprintf(fp, "✅ 테스트 로그 한 줄 추가!\n");
    fclose(fp);

    printf("log.txt에 기록이 저장되었습니다.\n");
    return 0;
}

