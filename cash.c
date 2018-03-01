#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main (void) {
    float n;
    do {
        n = get_float("amount due: ");
    } while ( n < 0);

    int cents = roundf(n * 100);
    int count = 0;
    int quarter = 25;
    int dime = 10;
    int nickel = 5;
    int penny = 1;
    int rem = 0;

    count = cents / quarter;
    rem = cents % quarter;
    count += rem / dime;
    rem = rem % dime;
    count += rem / nickel;
    rem = rem % nickel;
    count += rem / penny;


    printf("%i \n", count);

}