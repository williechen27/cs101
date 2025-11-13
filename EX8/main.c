#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define COUNTER_FILE "counter.bin"
#define MAX_LOTTO_NUM 7
#define MAX_LOTTO_NUMSET 5

// 程式步驟(1): 設定帳簿
void init_file() {
    int write_array[1] = {0};
    FILE* fp = fopen(COUNTER_FILE, "r");
    if (fp == NULL) {
        FILE* tmpfp = fopen(COUNTER_FILE, "wb+");
        fwrite(write_array, sizeof(int), 1, tmpfp);
        fclose(tmpfp);
    } else {
        fclose(fp);
    }
}

// 程式步驟(2): 讀取帳簿
int get_counter() {
    int read_array[1];
    FILE* tmpfp = fopen(COUNTER_FILE, "rb");
    fread(read_array, sizeof(int), 1, tmpfp);
    fclose(tmpfp);
    return read_array[0];
}

// 額外的3.1.1.1: 檢查 num 是否已在 numset[] 裡
int num_in_numset(int num, int numset[], int Len) {
    int ret = 0;
    for (int i = 0; i < Len; i++) {
        if (num == numset[i]) {
            ret = 1;
            break;
        }
    }
    return ret;
}

// 產生、排序並輸出一組樂透號碼
void print_lotto_row(FILE* tmpfp, int n) {
    int numset[MAX_LOTTO_NUM];

    // generate_lotto_row()
    fprintf(tmpfp, "[%d]: ", n);

    // 先產生前 6 碼 (1~69，不重複)
    for (int i = 0; i < MAX_LOTTO_NUM - 1; ) {
        int num = (rand() % 69) + 1;
        if (num_in_numset(num, numset, MAX_LOTTO_NUM - 1)) {
            continue;
        } else {
            numset[i] = num;
            i++;
        }
    }

    // 再產生特別號 (1~10，不與前 6 碼重複)
    for (int i = 0; i < 1; ) {
        int num = (rand() % 10) + 1;
        if (num_in_numset(num, numset, MAX_LOTTO_NUM - 1)) {
            continue;
        } else {
            numset[MAX_LOTTO_NUM - 1] = num;
            i++;
        }
    }

    // sorting_lotto_row()：只排序前 6 碼
    for (int i = 0; i < MAX_LOTTO_NUM - 1; ++i) {
        for (int j = 0; j < i; ++j) {
            if (numset[j] > numset[i]) {
                int temp  = numset[j];
                numset[j] = numset[i];
                numset[i] = temp;
            }
        }
    }

    // output_lotto_row：印出 7 個數字
    for (int i = 0; i < MAX_LOTTO_NUM; i++) {
        fprintf(tmpfp, "%02d ", numset[i]);
    }
    fprintf(tmpfp, "\n");
}

// 程式步驟(3): 輸出彩卷檔案
void print_lottofile(int num_set, int counter, char lotto_file[]) {
    time_t curtime;
    time(&curtime);
    srand(time(0));

    FILE* tmpfp = fopen(lotto_file, "w+");
    fprintf(tmpfp, "========= lotto649 =========\n");
    fprintf(tmpfp, "========+ No.%05d +========\n", counter);
    fprintf(tmpfp, "= %.24s =\n", ctime(&curtime));

    for (int i = 0; i < MAX_LOTTO_NUMSET; i++) {
        if (i < num_set) {
            print_lotto_row(tmpfp, i + 1);
        } else {
            fprintf(tmpfp, "[%d]: -- -- -- -- -- --\n", i + 1);
        }
    }

    fprintf(tmpfp, "========= csie@CGU =========\n");
    fclose(tmpfp);
}

void do_lotto_main(int counter) {
    char lotto_file[32];
    int num_set = 0;

    snprintf(lotto_file, 32, "lotto[%05d].txt", counter);
    printf("歡迎光臨長庚樂透採購買機台\n");
    printf("請問您要買幾組樂透彩：");
    scanf("%d", &num_set);
    print_lottofile(num_set, counter, lotto_file);
    printf("已為您購買的 %d 組樂透組合輸出至 %s\n", num_set, lotto_file);
}

void set_counter(int counter) {
    int write_array[1];
    write_array[0] = counter;
    FILE *tmpfp = fopen(COUNTER_FILE, "wb");
    fwrite(write_array, sizeof(int), 1, tmpfp);
    fclose(tmpfp);
}

int main() {
    int counter;
    init_file();                    // 初始化紀錄檔
    counter = get_counter();
    printf("counter = %d\n", counter);
    do_lotto_main(++counter);
    set_counter(counter);           // 寫回紀錄檔
    return 0;
}
