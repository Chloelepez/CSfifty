#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main (void) {
    long long int cc;
    do {
        cc = get_long_long("Number: ");
    } while ( cc < 0);

    //checksum
    int length = (int)log10(cc) + 1;
    int arr[length];
    int long long check = cc;

    for (int i = 0; i < length; i++) {
        arr[i] = cc % 10;
        cc /= 10;
    }

    int sum = 0;

    for (int j = 0; j < length; j++) {
        if (j % 2 == 0) {
            sum += arr[j];
        }

        else if (j % 2 != 0) {
            arr[j] *= 2;
            if (arr[j] >= 10) {
                int doz = arr[j] /10;
                sum += doz;
                }

            int mod = arr[j] % 10;
            sum += mod;
        }
    }


    if (sum % 10 == 0) {

    //validate company identifier
        if ((check >= 340000000000000 && check < 350000000000000) || (check >= 370000000000000 && check < 380000000000000) ) {
            printf("AMEX\n");
        } else if ( check >= 5100000000000000 && check < 5600000000000000 ) {
             printf("MASTERCARD\n");
        } else if ( (check >= 4000000000000 && check < 5000000000000) || (check >= 4000000000000000 && check < 5000000000000000) ) {
            printf("VISA\n");
        } else {
                printf("INVALID\n");
        }

    } else {
            printf("INVALID\n");
        }
}