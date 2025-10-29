/*#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main(void)
{
    char input[200];
    printf("문장을 입력하세요: ");
    fgets(input, sizeof(input), stdin); // 한 줄 입력받기

    // 줄바꿈(\n) 제거 (fgets는 엔터도 같이 읽음)
    input[strcspn(input, "\n")] = '\0';

    // 비교하기
    if (strcmp(input, "교수님은 초래를 좋아할까") == 0)
    {
        strcpy(input, "교수님은 초래만 좋아해");
    }

    printf("결과: %s\n", input);
    return 0;
}*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main(void)
{
    char input[200];
    printf("문장을 입력하세요: ");
    fgets(input, sizeof(input), stdin);  // 한 줄 입력 받기

    char buf[200];
    strcpy(buf, input);

    // "바보"를 찾을 위치 포인터
    char* q = buf;

    // 문자열 안에 "바보"가 있는 동안 반복
    while ((q = strstr(q, "바보")) != NULL)
    {
        // "바보"는 UTF-8에서 6바이트, "**"는 2바이트
        // 일단 뒤쪽 문자열을 당기거나 늘리기 전에 안전하게 처리
        memmove(q + strlen("**"), q + strlen("바보"),
            strlen(q + strlen("바보")) + 1);

        // "바보" 대신 "**" 쓰기
        memcpy(q, "**", strlen("**"));

        // 다음 검색 위치를 바꿔치기한 부분 뒤로 이동
        q += strlen("**");
    }

    printf("‘바보’ → ‘**’ 결과: %s\n", buf);
    return 0;
}

