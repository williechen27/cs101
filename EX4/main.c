#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    int arr_write[5] = {0, 1, 2, 3, 4};
    int arr_read[5];

    if ((fp = fopen("cj.bin", "wb+")) == NULL) {
        printf("Can not open the file ...\n");
        exit(1);
    }

    if (fwrite(arr_write, sizeof(arr_write), 1, fp) != 1) {
        printf("Write file error\n");
    }

    fseek(fp, 0, SEEK_SET);

    if (fread(arr_read, sizeof(arr_read), 1, fp) != 1) {
        printf("Read file error\n");
    }

    for (int i = 0; i < 5; i++) {
        printf("%d ", arr_read[i]);
    }
    printf("\n");

    fclose(fp);
    return 0;
}
