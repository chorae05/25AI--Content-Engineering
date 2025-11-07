#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*1. 입력 파일 형식 (students.csv)

총 10명, 각 줄은 한 명의 학생 정보를 의미한다.
형식: 이름,공격력,방어력,HP (쉼표 구분)
ANSI 인코딩으로 저장되어있음

2. 처리 요구사항 (정확한 출력 포맷)

프로그램은 students.csv를 읽고 아래 순서대로 Test.txt 파일에 내용을 출력해야 한다.
출력 형식은 반드시 일치해야 한다.

(1) 첫 번째 줄

시험 응시자가 직접 입력한 학번과 이름을 출력한다.
프로그램 실행 시 scanf로 입력받는다.


(2) 두 번째 줄

CSV에서 가장 공격력이 높은 사람을 찾아 출력한다.
ex>
가장 공격력이 높은 사람: 이름 (ATK=공격력)

(공격력이 같다면 CSV에서 먼저 등장한 사람을 출력)


*/

typedef struct {
    char name[20];
    int atk;
    int def;
    int hp;
} Student;


void user_name() {
    int hakbun;
    char name[20];

    // 1. 학번과 이름 입력받기
    printf("학번을 입력하세요: ");
    scanf_s("%d", &hakbun);
    printf("이름을 입력하세요: ");
    scanf_s("%s", name, (rsize_t)sizeof(name));
}

int csv_r() {

    char line[100];
    int count = 0;

    FILE* fp = NULL;
    fopen_s(&fp, "students.csv", "r");
    if (!fp) {
        printf("students.csv 파일을 열 수 없습니다.\n");
        return 1;
    }

    while (fgets(line, sizeof(line), fp) != NULL && count < 10) {
        char* ctx = NULL;
        char* tok = strtok_s(line, ",", &ctx);
        strcpy_s(s[count].name, sizeof(s[count].name), tok);

        tok = strtok_s(NULL, ",", &ctx);
        s[count].atk = atoi(tok);
        tok = strtok_s(NULL, ",", &ctx);
        s[count].def = atoi(tok);
        tok = strtok_s(NULL, ",", &ctx);
        s[count].hp = atoi(tok);

        count++;
    }
    fclose(fp);
    return count;
}

int fight(Student a, Student b)
{
    int dmgA = b.atk - a.def;
    int dmgB = a.atk - b.def;
    if (dmgA < 0) dmgA = 0;
    if (dmgB < 0) dmgB = 0;

    if (dmgA == 0 && dmgB == 0) return 0;

    while (a.hp > 0 && b.hp > 0) {
        a.hp -= dmgA;
        b.hp -= dmgB;
    }

    if (a.hp <= 0 && b.hp <= 0) return 0;
    else if (a.hp > 0) return 1;
    else return 2;
}

int findStrongest(Student s[], int count)
{
    int max = 0;
    for (int i = 1; i < count; i++)
        if (s[i].atk > s[max].atk) max = i;
    return max;
}



int main() {
    int hakbun;     //콘솔창에 출력되는 학번
    char name[20];  //콘솔창에 출력되는 이름
    Student s[10];  //구조체에서 정한 인원 수
    int count;      //학생수를 세기 위해 count 적음

    getUserInfo(&hakbun, name, sizeof(name)); //사용자 정보, main에서 순서대로 호출하는 
    count = readCSV("students.csv", s);
    writeResult("Test.txt", hakbun, name, s, count);



}

