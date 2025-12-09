#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"

// =======================================
// 퍼즐 CSV 구조체 및 공용 변수
// =======================================

typedef struct {
    int  id;
    char name[50];
    char slot[10];
    int  atk;
    int  def;
    int  hp;
    char curse[50];
    char key_frag[50];
} PuzzleItem;

#define MAX_ITEMS    100
#define MAX_LINE_LEN 256

static PuzzleItem p_items[MAX_ITEMS];
static int  p_count = 0;
static char current_filename[100] = { 0 };

// =======================================
// 퍼즐 CSV 로딩 (파일명 후보 여러 개 시도)
// =======================================

static void load_csv_data(void) {
    const char* candidates[] = {
        "game_puzzle_en.csv",
        "AI1-2_C_Final.csv",
        "puzzle.csv",
        "data.csv"
    };
    FILE* fp = NULL;
    int num_candidates = (int)(sizeof(candidates) / sizeof(candidates[0]));

    for (int i = 0; i < num_candidates; i++) {
        fp = fopen(candidates[i], "r");
        if (fp) {
            strcpy(current_filename, candidates[i]);
            printf("SYSTEM: CSV Loaded [%s]\n", current_filename);
            break;
        }
    }
    if (!fp) {
        printf("ERROR: puzzle CSV file not found!\n");
        return;
    }

    char line[MAX_LINE_LEN];
    // 헤더 1줄 스킵
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return;
    }

    p_count = 0;
    while (fgets(line, sizeof(line), fp) && p_count < MAX_ITEMS) {
        PuzzleItem* it = &p_items[p_count];
        char temp[MAX_LINE_LEN];
        strcpy(temp, line);

        char* tok = strtok(temp, ","); if (!tok) continue;
        it->id = atoi(tok);

        tok = strtok(NULL, ","); if (!tok) continue;
        strcpy(it->name, tok);

        tok = strtok(NULL, ","); if (!tok) continue;
        strcpy(it->slot, tok);

        tok = strtok(NULL, ","); if (!tok) continue;
        it->atk = atoi(tok);

        tok = strtok(NULL, ","); if (!tok) continue;
        it->def = atoi(tok);

        tok = strtok(NULL, ","); if (!tok) continue;
        it->hp = atoi(tok);

        tok = strtok(NULL, ","); if (!tok) continue;
        strcpy(it->curse, tok);

        tok = strtok(NULL, ","); if (!tok) continue;
        tok[strcspn(tok, "\r\n")] = 0;  // 개행 제거
        strcpy(it->key_frag, tok);

        p_count++;
    }
    fclose(fp);
}

// id로 퍼즐 아이템 찾기
static PuzzleItem* get_item(int id) {
    for (int i = 0; i < p_count; i++) {
        if (p_items[i].id == id) return &p_items[i];
    }
    return NULL;
}

// =======================================
// 각 문제별 정답 생성 함수들
// =======================================

// 6번: 독 (POISON)
static void solve_poison(char* buf) {
    buf[0] = 0;
    int idx[MAX_ITEMS];
    int c = 0;

    for (int i = 0; i < p_count; i++) {
        if (p_items[i].atk >= 4 &&
            p_items[i].def <= 5 &&
            p_items[i].hp <= 100) {
            idx[c++] = i;
        }
    }
    for (int i = c - 1; i >= 0; i--) {
        strcat(buf, p_items[idx[i]].name);
        if (i > 0) strcat(buf, "|");
    }
}

// 7번: 강타 (STRIKE)
static void solve_strike(char* buf) {
    int sum = 0;
    for (int i = 0; i < p_count; i++) {
        if (strcmp(p_items[i].slot, "W") == 0) {
            char* p = strchr(p_items[i].key_frag, 'T');
            if (p) {
                sum += (int)(p - p_items[i].key_frag);
            }
        }
    }
    sprintf(buf, "%dkey", sum);
}

// 8~11번: 점멸 (BLINK)
static void solve_blink(char* buf) {
    buf[0] = 0;
    PuzzleItem* i202 = get_item(202);
    PuzzleItem* i208 = get_item(208);
    PuzzleItem* i205 = get_item(205);
    PuzzleItem* i212 = get_item(212);
    if (!i202 || !i208 || !i205 || !i212) return;

    // 1) HP 합 일치 + NIL 제외 → 마지막 것
    int thp = i202->def + i208->def;
    char* key1 = "";
    for (int i = 0; i < p_count; i++) {
        if (p_items[i].hp == thp &&
            strcmp(p_items[i].key_frag, "NIL") != 0) {
            key1 = p_items[i].key_frag;
        }
    }
    strcat(buf, key1);

    // 2) ATK 곱 일치 + NIL 제외 → 마지막 것
    int tatk = i205->atk * i212->atk;
    char* key2 = "";
    for (int i = 0; i < p_count; i++) {
        if (p_items[i].atk == tatk &&
            strcmp(p_items[i].key_frag, "NIL") != 0) {
            key2 = p_items[i].key_frag;
        }
    }
    strcat(buf, key2);

    // 3) curse에 C_01 포함 + NIL 제외 → 마지막 것
    int fidx = -1;
    for (int i = 0; i < p_count; i++) {
        if (strstr(p_items[i].curse, "C_01") &&
            strcmp(p_items[i].key_frag, "NIL") != 0) {
            fidx = i;
        }
    }
    if (fidx != -1) {
        strcat(buf, p_items[fidx].key_frag);
    }

    // 4) 이름이 'I'로 시작 + NIL 제외 → 첫 번째 것
    for (int i = 0; i < p_count; i++) {
        if (p_items[i].name[0] == 'I' &&
            strcmp(p_items[i].key_frag, "NIL") != 0) {
            strcat(buf, p_items[i].key_frag);
            break;
        }
    }
}

// 13번: 회복2 (HEAL_ALL)
static void solve_heal_all(char* buf) {
    buf[0] = 0;
    for (int i = 0; i < p_count; i++) {
        if (strcmp(p_items[i].name, p_items[i].slot) >= 0) {
            strcpy(buf, p_items[i].key_frag);
            return;
        }
    }
}

// 14번: 원거리 공격 (RANGE_ATTACK) - fseek(nv-1) 후 5바이트
static void solve_range(char* buf) {
    buf[0] = 0;
    int nv = 0;

    for (int i = 0; i < p_count; i++) {
        if (strstr(p_items[i].key_frag, "K") != NULL) {
            nv = p_items[i].hp;
            break;
        }
    }
    if (nv <= 0 || current_filename[0] == '\0') return;

    FILE* fp = fopen(current_filename, "rb");
    if (!fp) return;

    if (fseek(fp, nv - 1, SEEK_SET) == 0) {
        char t[6] = { 0 };
        size_t r = fread(t, 1, 5, fp); // 최대 5바이트 읽기
        t[r] = '\0';
        sprintf(buf, "\"%s\"", t);
    }
    fclose(fp);
}

// 16번: 축복 (BLESS) - 원래 자폭(Self Destruct) 퍼즐
// Sword 포함 + NIL 제외 → 전부 이어붙인 뒤, '*' 기준 가장 긴 조각
static void solve_self_destruct(char* buf) {
    buf[0] = 0;
    char all[2000] = { 0 };

    for (int i = 0; i < p_count; i++) {
        if (strstr(p_items[i].name, "Sword") &&
            strcmp(p_items[i].key_frag, "NIL") != 0) {
            strcat(all, p_items[i].key_frag);
        }
    }

    char* tok = strtok(all, "*");
    char* best = NULL;
    int   best_len = -1;

    while (tok) {
        int len = (int)strlen(tok);
        if (len > best_len) {
            best_len = len;
            best = tok;
        }
        tok = strtok(NULL, "*");
    }

    if (best) {
        strcpy(buf, best);
    }
}

// 17/18번: 가로/세로 공격 (H/V_ATTACK)
// 이름 가장 긴 아이템 + curse 가장 짧은 아이템
// → name 앞 3글자 + curse 뒤 3글자
static void solve_hv(char* buf) {
    buf[0] = 0;
    int max_name_len = -1, min_curse_len = 999;
    int idx_name = -1, idx_curse = -1;

    for (int i = 0; i < p_count; i++) {
        int nl = (int)strlen(p_items[i].name);
        int cl = (int)strlen(p_items[i].curse);
        if (nl > max_name_len) { max_name_len = nl; idx_name = i; }
        if (cl < min_curse_len) { min_curse_len = cl; idx_curse = i; }
    }

    if (idx_name == -1 || idx_curse == -1) return;

    char p1[4] = { 0 }, p2[4] = { 0 };
    strncpy(p1, p_items[idx_name].name, 3);
    int cl = (int)strlen(p_items[idx_curse].curse);
    if (cl >= 3) {
        strncpy(p2, p_items[idx_curse].curse + (cl - 3), 3);
    }
    else {
        strcpy(p2, p_items[idx_curse].curse);
    }
    sprintf(buf, "%s%s", p1, p2);
}

// 19번: 히든 (SECRETE) - Stone 이름에서 모음 기준 최대 조각
static void solve_secret(char* buf) {
    buf[0] = 0;
    char tn[50] = { 0 };

    for (int i = 0; i < p_count; i++) {
        if (strstr(p_items[i].name, "Stone")) {
            strcpy(tn, p_items[i].name);
            break;
        }
    }
    if (tn[0] == '\0') return;

    char* tok = strtok(tn, "AEIOUaeiou");
    char* best = NULL;
    int   best_len = -1;

    while (tok) {
        int len = (int)strlen(tok);
        if (len > best_len) {
            best_len = len;
            best = tok;
        }
        tok = strtok(NULL, "AEIOUaeiou");
    }

    if (best) {
        strcpy(buf, best);
    }
}

// =======================================
// 해금만 하는 엔트리 + AI 껍데기
// =======================================

static int my_secret_key = 0;

// 팀원들이 알아서 갈아끼우면 되는 AI 자리


    // AI 로직 함수 (CommandFn 형태 준수)
static int my_ai(const Player* my_info, const Player* opponent_info) {
    int my_hp = get_player_hp(my_info);
    int my_mp = get_player_mp(my_info);
    int opp_hp = get_player_hp(opponent_info);

    // --- API Getter 함수만 사용하여 정보 조회 ---
    int my_x = get_player_x(my_info);
    int my_y = get_player_y(my_info);
    int opp_x = get_player_x(opponent_info);
    int opp_y = get_player_y(opponent_info);

    // 맨하탄 거리 계산 (stdlib의 abs() 사용)
    int manhattan_distance = abs(my_x - opp_x) + abs(my_y - opp_y);

    // -----------------------------------------------
    // 1. 생존 및 방어 (최우선)
    // -----------------------------------------------

    // 1-A. 긴급 회복: HP가 3 이하일 때 (MP 2 소모)
    if (my_hp <= 3 && my_mp >= 2 && is_skill_unlocked(my_secret_key, CMD_HEAL_ALL)) {
        return CMD_HEAL_ALL;
    }

    // 1-B. 독 해제: 나에게 독이 걸렸다면 MP 2를 써서 해제 (독 상태 Getter 부재 시, 이 로직은 불완전)
    // NOTE: 현재 API 정보만으로는 독 지속시간을 알 수 없으므로, 이 코드는 임시로 제외하거나,
    //       독에 걸린 것을 확인하는 가상의 Getter가 있다고 가정해야 합니다.
    /* if (get_player_poison_duration(my_info) > 0 && my_mp >= 2 && is_skill_unlocked(my_secret_key, CMD_BLESS)) {
        return CMD_BLESS;
    }
    */

    // -----------------------------------------------
    // 2. 자원 확보 (MP 5 확보 목표)
    // -----------------------------------------------

    // MP가 5 미만이면 휴식 (CMD_REST는 MP 2 회복). 공격/회복 스킬 준비
    // or
    if (my_mp < 5) {
        return CMD_REST;
    }

    // -----------------------------------------------
    // 3. 공격 (마무리 및 압박) - MP 5 확보 상태
    // -----------------------------------------------

    // 3-A. 확정 킬: 강타(Damage 2)로 상대를 끝낼 수 있을 때 (거리 1 이내, MP 2 이상)
    if (opp_hp <= 2 && my_mp >= 2 && manhattan_distance <= 1 && is_skill_unlocked(my_secret_key, CMD_STRIKE)) {
        return CMD_STRIKE;
    }

    // 3-B. 선제 독 공격: MP 5 소모로 압박 시작 (독 상태 확인 불가 시, MP 5일 때 무조건 씀)
    if (my_mp == 5 && is_skill_unlocked(my_secret_key, CMD_POISON)) {
        return CMD_POISON;
    }

    // 3-C. 거리 2에서 원거리 공격 (MP 1 소모, 좋은 견제)
    if (manhattan_distance == 2 && my_mp >= 1 && is_skill_unlocked(my_secret_key, CMD_RANGE_ATTACK)) {
        return CMD_RANGE_ATTACK;
    }

    // -----------------------------------------------
    // 4. 이동 및 접근
    // -----------------------------------------------

    // 공격 사거리 2 밖이라면 접근 (MP가 5이므로 점멸 우선 사용)
    if (manhattan_distance > 2) {
        // X축/Y축 중 거리가 더 먼 쪽을 먼저 이동하여 빠르게 접근
        int dx = abs(my_x - opp_x);
        int dy = abs(my_y - opp_y);

        if (dx >= dy) { // X축 이동 (좌/우)
            int target_blink = (my_x < opp_x) ? CMD_BLINK_RIGHT : CMD_BLINK_LEFT;
            int target_move = (my_x < opp_x) ? CMD_RIGHT : CMD_LEFT;

            if (is_skill_unlocked(my_secret_key, target_blink) && my_mp >= 1) return target_blink;
            return target_move;
        }
        else { // Y축 이동 (상/하)
            int target_blink = (my_y < opp_y) ? CMD_BLINK_DOWN : CMD_BLINK_UP;
            int target_move = (my_y < opp_y) ? CMD_DOWN : CMD_UP;

            if (is_skill_unlocked(my_secret_key, target_blink) && my_mp >= 1) return target_blink;
            return target_move;
        }
    }

    // -----------------------------------------------
    // 5. 기본 공격 (모든 조건 불충족 시)
    // -----------------------------------------------

    // 근접했다면 일반 공격
    if (manhattan_distance <= 1) {
        return CMD_ATTACK;
    }

    return CMD_REST; // 모든 행동이 불가능할 경우 휴식
}



void student1_ai_entry(void) {
    load_csv_data(); // 퍼즐 CSV 로딩

    my_secret_key = register_player_ai("3_Team", my_ai);

    char ans[256];

    // 6번: POISON
    solve_poison(ans);
    attempt_skill_unlock(my_secret_key, CMD_POISON, ans);

    // 7번: STRIKE
    solve_strike(ans);
    attempt_skill_unlock(my_secret_key, CMD_STRIKE, ans);

    // 8~11번: BLINK 계열
    solve_blink(ans);
    attempt_skill_unlock(my_secret_key, CMD_BLINK_UP, ans); // BLINK 4종 한꺼번에 해금

    // 13번: HEAL_ALL
    solve_heal_all(ans);
    attempt_skill_unlock(my_secret_key, CMD_HEAL_ALL, ans);

    // 14번: RANGE_ATTACK
    solve_range(ans);
    attempt_skill_unlock(my_secret_key, CMD_RANGE_ATTACK, ans);

    // 16번: BLESS (원래 SELF_DESTRUCT 퍼즐)
    solve_self_destruct(ans);
    attempt_skill_unlock(my_secret_key, CMD_BLESS, ans);

    // 17/18번: H/V_ATTACK
    solve_hv(ans);
    attempt_skill_unlock(my_secret_key, CMD_H_ATTACK, ans); // H/V 둘 다 같이 해금

    // 19번: SECRETE
    solve_secret(ans);
    attempt_skill_unlock(my_secret_key, CMD_SECRETE, ans);

    // 필요하면 여기서 is_skill_unlocked로 디버깅 출력 가능
    // 필요하면 여기서 is_skill_unlocked로 디버깅 출력 가능
    printf("==== UNLOCK CHECK ====\n");
    printf("POISON: %d\n", is_skill_unlocked(my_secret_key, CMD_POISON));
    printf("STRIKE: %d\n", is_skill_unlocked(my_secret_key, CMD_STRIKE));
    printf("BLINK_UP: %d\n", is_skill_unlocked(my_secret_key, CMD_BLINK_UP));
    printf("HEAL_ALL: %d\n", is_skill_unlocked(my_secret_key, CMD_HEAL_ALL));
    printf("RANGE: %d\n", is_skill_unlocked(my_secret_key, CMD_RANGE_ATTACK));
    printf("BLESS: %d\n", is_skill_unlocked(my_secret_key, CMD_BLESS));
    printf("HV: %d\n", is_skill_unlocked(my_secret_key, CMD_H_ATTACK));
    printf("SECRET: %d\n", is_skill_unlocked(my_secret_key, CMD_SECRETE));
    printf("=======================\n");


}
