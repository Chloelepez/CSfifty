#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


int main(int argc, string argv[]) {

    if (argc != 2) {
       printf("Error\n");
       return 1;
    }

    string k = argv[1];
    int lentk = strlen(k);
    for (int n = 0; n < lentk ; n++) {
        if (!isalpha(k[n])) {
            printf("Error\n");
            return 1;
        }
    }


    string plaintext = get_string("plaintext: ");
    int len = strlen(plaintext);
    char cipher[len+ 1];
    cipher[strlen(plaintext)] = '\0';
    
    int j = 0;


    for (int i = 0; i < len; i++) {

        if (isalpha(plaintext[i])) {

            int letterKey = tolower(k[j % lentk]) - 'a';

            if (isupper(plaintext[i])) {
                    cipher[i] = (((plaintext[i] + letterKey) - 'A') % 26) + 'A';

            } else if (islower(plaintext[i])) {

                cipher[i] = (((plaintext[i] + letterKey) - 'a') % 26) + 'a';
            }

            j++;

        } else {

            cipher[i] = plaintext[i];
            }

    }

    printf("ciphertext: %s\n", cipher);

}