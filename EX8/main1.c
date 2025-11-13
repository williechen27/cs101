#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

/* 工具：把 "========  March 13 2025  ========" -> "March 13 2025" */
static void clean_date_line(char *s) {
    char t[256]; int j=0, in_space=1;
    for (int i=0; s[i]; ++i) {
        if (s[i] == '=') continue;                              // 去掉 '='
        if (isspace((unsigned char)s[i])) {                     // 壓縮空白
            if (!in_space) { t[j++] = ' '; in_space = 1; }
        } else {
            t[j++] = s[i]; in_space = 0;
        }
    }
    while (j>0 && isspace((unsigned char)t[j-1])) j--;          // 去尾空白
    t[j] = '\0';
    strcpy(s, t);
}

/* 取不重複且排序的 k 個號碼（範圍 lo..hi) */
static void pick_unique_sorted(int *out, int k, int lo, int hi) {
    int n = 0;
    while (n < k) {
        int v = rand() % (hi - lo + 1) + lo;
        int dup = 0; for (int i=0;i<n;i++) if (out[i]==v) { dup=1; break; }
        if (!dup) out[n++] = v;
    }
    for (int i=0;i<k-1;i++) for (int j=i+1;j<k;j++)
        if (out[i] > out[j]) { int t=out[i]; out[i]=out[j]; out[j]=t; }
}

/* 產生符合 PPT 截圖的 lotto.txt */
static int gen_lotto_txt(const char *filename) {
    const char *MONTH[] = {"January","February","March","April","May","June",
                           "July","August","September","October","November","December"};
    srand((unsigned)time(NULL));

    FILE *fp = fopen(filename, "w");
    if (!fp) return 0;

    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    const char *mon = MONTH[tm->tm_mon];
    int day = tm->tm_mday, year = tm->tm_year + 1900;

    // 第一行與第二行（只有 "Month Day Year"）
    fprintf(fp, "======== lotto649 ========\n");
    fprintf(fp, "========  %s %d %d  ========\n", mon, day, year);

    // 1~5 張，未售以 "--" 補；號碼 1..69、7 顆、兩位數
    int sold = (rand() % 5) + 1;
    for (int i=1;i<=5;i++) {
        if (i <= sold) {
            int n[7]; pick_unique_sorted(n, 7, 1, 69);
            fprintf(fp, "[%d]: %02d %02d %02d %02d %02d %02d %02d\n",
                    i, n[0],n[1],n[2],n[3],n[4],n[5],n[6]);
        } else {
            fprintf(fp, "[%d]: -- -- -- -- -- -- --\n", i);
        }
    }
    fprintf(fp, "======== csie@CGU ========\n");
    fclose(fp);
    return 1;
}

/* 主程式：若無檔則先生成 -> 輸入三碼 -> 列出命中彩券 */
int main(void) {
    const char *fname = "lotto.txt";
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        if (!gen_lotto_txt(fname)) { puts("無法建立 lotto.txt"); return 1; }
        fp = fopen(fname, "r");
        if (!fp) { puts("打開 lotto.txt 失敗"); return 1; }
    }

    int a,b,c;
    printf("請輸入中獎號碼三個：");
    if (scanf("%d %d %d", &a, &b, &c) != 3) { puts("輸入格式錯誤"); fclose(fp); return 1; }
    printf("輸入中獎號碼為：%02d %02d %02d\n", a, b, c);

    // 讀標題與日期行
    char line[256], date_line[256]="";
    if (!fgets(line, sizeof(line), fp)) { fclose(fp); return 0; }   // "======== lotto649 ========"
    if (fgets(line, sizeof(line), fp)) {
        strncpy(date_line, line, sizeof(date_line)-1);
        date_line[sizeof(date_line)-1] = '\0';
        clean_date_line(date_line);                                  // -> "March 13 2025"
    }

    puts("以下為中獎彩卷：");

    // 解析 [1]..[5]
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "csie@CGU")) break;     // 尾行
        if (strstr(line, "--")) continue;        // 未售票

        int idx, n[7];
        int got = sscanf(line, " [%d]: %d %d %d %d %d %d %d",
                         &idx,&n[0],&n[1],&n[2],&n[3],&n[4],&n[5],&n[6]);
        if (got == 8) {
            int hit = 0;
            for (int i=0;i<7;i++) if (n[i]==a || n[i]==b || n[i]==c) { hit=1; break; }
            if (hit) {
                printf("售出時間： %s: [%d]: %02d %02d %02d %02d %02d %02d %02d\n",
                       date_line, idx, n[0],n[1],n[2],n[3],n[4],n[5],n[6]);
            }
        }
    }
    fclose(fp);
    return 0;
}
