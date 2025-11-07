#include <stdio.h>   // 화면에 글자를 출력하거나 입력받을 때 사용하는 기본 헤더파일
#include <stdlib.h>  // 숫자 바꾸기(atoi 등), 메모리 관련 함수 사용
#include <string.h>  // 문자열(글자)을 다룰 때 필요한 함수들
#include <conio.h>   // 콘솔 입력 관련 함수들 (여기선 사실 안 써도 돼요)

// 한 번에 저장할 수 있는 최대 학생 수
#define MAX 100

// 학생 한 명의 정보를 저장할 구조체를 만듭니다.
// 이름(name), 공격력(atk), 방어력(def), 체력(hp)을 한 묶음으로 관리할 수 있어요!
typedef struct {
    char name[64]; // 이름 (글자 최대 63자 + 마지막에 \0)
    int atk;       // 공격력
    int def;       // 방어력
    int hp;        // 체력
} Student;

int main(void) // 프로그램이 시작되는 부분
{
    Student stu[10]; // 학생 정보를 담을 구조체 배열 (최대 10명까지 저장)
    char line[256];  // 파일에서 한 줄씩 읽을 공간
    int count = 0;   // 현재 읽은 학생 수를 세는 변수
    char id[20];     // 사용자의 학번 저장
    char user_name[64]; // 사용자의 이름 저장
    char winner[64];    // 싸움에서 이긴 사람의 이름을 저장

    // 🟢 사용자에게 학번을 입력받기
    printf("학번을 입력하세요: ");
    scanf_s("%s", id, (rsize_t)sizeof(id)); // 학번을 문자열로 입력받아요.

    // 🟢 사용자에게 이름을 입력받기
    printf("이름을 입력하세요: ");
    scanf_s("%s", user_name, (rsize_t)sizeof(user_name)); // 이름을 입력받아요.

    // 🟢 학생 정보를 저장한 파일(students.csv)을 엽니다.
    FILE* fp = NULL; // 파일을 가리킬 변수 (파일 포인터)
    fopen_s(&fp, "students.csv", "r"); // 읽기 모드("r")로 파일 열기

    // 만약 파일이 열리지 않으면 (파일이 없거나 경로가 틀렸을 때)
    if (!fp) {
        printf("students.csv 파일을 열 수 없습니다.\n");
        return 1; // 프로그램을 종료합니다.
    }

    // 🟢 파일에서 한 줄씩 읽어서 학생 정보를 저장합니다.
    while (fgets(line, sizeof(line), fp) && count < 10) {
        // fgets로 한 줄 읽기 → 예: "김철수,80,70,100"
        char* t, * n; // 문자열 자르기에 사용할 임시 변수

        // 첫 번째 데이터(이름)를 자릅니다.
        t = strtok_s(line, ",", &n); // ,(콤마)를 기준으로 잘라서 첫 단어 가져오기
        strcpy_s(stu[count].name, sizeof(stu[count].name), t); // 이름 저장

        //stu[count]: 지금까지 몇 명의 학생을 읽었는지 세는 숫자
        //atoi: “문자열을 숫자(int)로 바꿔주는 함수” 
        // 
        // 두 번째 데이터(공격력)
        t = strtok_s(NULL, ",", &n); // 다음 콤마 뒤의 값 가져오기
        stu[count].atk = atoi(t);    // 문자열을 숫자로 바꿔서 공격력에 저장

        // 세 번째 데이터(방어력)
        t = strtok_s(NULL, ",", &n);
        stu[count].def = atoi(t);

        // 네 번째 데이터(체력)
        t = strtok_s(NULL, ",", &n);
        stu[count].hp = atoi(t);

        count++; // 학생 한 명을 다 읽었으니 수를 1 증가시킴
    }

    fclose(fp); // 파일 닫기 (이제 다 읽었어요)


    // 🟢 공격력이 가장 높은 학생 찾기
    int max_idx = 0; // 첫 번째 학생부터 시작
    for (int i = 1; i < count; i++) // 나머지 학생들과 비교
        if (stu[i].atk > stu[max_idx].atk) // 더 높은 공격력을 가진 학생이 있으면
            max_idx = i; // 그 학생의 번호(i)를 저장

    // 🟢 싸움할 두 사람 정하기
    // 예: 3번째(index=3) vs 8번째(index=8)
    int i3 = 3, i8 = 8;

    // 🥊 싸움 계산하기
    // 상대의 공격력에서 내 방어력을 뺀 값이 실제로 입히는 데미지
    int damage3 = stu[i8].atk - stu[i3].def; // 8번째가 3번째에게 주는 데미지
    int damage8 = stu[i3].atk - stu[i8].def; // 3번째가 8번째에게 주는 데미지
    if (damage3 < 0) damage3 = 0; // 마이너스 데미지는 0으로 처리
    if (damage8 < 0) damage8 = 0;

    // ⚔️ 싸움 결과 계산하기
    int hp3 = stu[i3].hp - damage3;
    int hp8 = stu[i8].hp - damage8;

    if (damage3 == 0 && damage8 == 0)
        strcpy_s(winner, sizeof(winner), "무승부");
    else if (hp3 == hp8)
        strcpy_s(winner, sizeof(winner), "무승부");
    else if (hp3 > hp8)
        strcpy_s(winner, sizeof(winner), stu[i3].name);
    else
        strcpy_s(winner, sizeof(winner), stu[i8].name);

    // 🟢 결과를 저장할 파일(Test.txt)을 엽니다.
    FILE* out = NULL;
    fopen_s(&out, "Test.txt", "w"); // 쓰기 모드("w")로 파일 열기

    if (!out) { // 만약 파일을 못 열면
        printf("Test.txt 파일을 쓸 수 없습니다.\n");
        return 1; // 프로그램 종료
    }

    // 파일에 결과를 저장하기
    fprintf(out, "%s : %s\n", id, user_name); // 사용자의 학번과 이름
    fprintf(out, "가장 공격력이 높은 사람: %s (ATK=%d)\n", stu[max_idx].name, stu[max_idx].atk);
    fprintf(out, "3번째: %s HP=%d\n", stu[i3].name, stu[i3].hp);
    fprintf(out, "8번째: %s HP=%d\n", stu[i8].name, stu[i8].hp);

    // 싸움 결과 출력
    if (strcmp(winner, "무승부") == 0) //winner 변수 안의 문자열이 "무승부"와 같은지 비교
        fprintf(out, "3번째 vs 8번째: 무승부\n");
    else
        fprintf(out, "3번째 vs 8번째 승자: %s\n", winner);

    // 마지막 줄
    fprintf(out, "교수님 시험문제 너무 쉽습니다. 담주에 더 어렵게 내주세요");

    fclose(out); // 파일 닫기

    // 🟢 화면에 알림 출력
    printf("결과가 Test.txt로 저장되었습니다!\n");

    return 0; // 프로그램 정상 종료
}
