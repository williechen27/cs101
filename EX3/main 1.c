#include <stdio.h>

void print_sp(int i, int n) {
    for(int s=0; s<n-i; s++) {
        printf(" ");
    }
}

void print_num(int n) {
    for(int j=0; j<n; j++) {
        printf("%d ", n);
    }
}

int main() {
    int rows = 6;
    for(int i=1; i<=rows; i++) {
        print_sp(i, rows);
        print_num(i);
        printf("\n");
    }
    return 0;
}
