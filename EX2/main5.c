#include <stdio.h>

int main() {
    char grade = 'B';
    printf("Your grade is %c\n", grade);
    switch(grade) {
        case 'A':
            printf("Excellent!\n");
            break;
        case 'B':
        case 'C':
            printf("Well done!\n");
            break;
        case 'D':
        case 'E':
            printf("Better try again!\n");
            break;
        default:
            printf("Invali grade\n");
    }
    return 0;
}
