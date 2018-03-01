#include <stdio.h>
#include <cs50.h>

int main (void) {
    int n;
    do {
        n = get_int("give me an int: ");
    } while ( n < 0 || n > 23);


    for (int i = 0; i < n; i++) {

        for (int j = n - i - 1; j > 0; j--) {
            printf(" ");

        }
        for (int k = n - i - 1; k < n; k++) {
                printf("#");
            }


        printf("  ");


        for (int m = n - i - 1 ; m < n; m++) {
            printf("#");

        }


        printf("\n");
    }
}