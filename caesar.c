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

    int key = atoi(argv[1]);

    if (key < 0) {
        printf("Error\n");
        return 1;

    } else {

        string plaintext = get_string("plaintext: ");
        int len = strlen(plaintext);
        char cipher[len];


        for (int i = 0; i < len; i++) {

            if (isalpha(plaintext[i])) {

                if (isupper(plaintext[i])) {

                    cipher[i] = (((plaintext[i] + key) - 'A') % 26) + 'A';

                } else if (islower(plaintext[i])) {

                    cipher[i] = (((plaintext[i] + key) - 'a') % 26) + 'a';
                }

            } else {

                cipher[i] = plaintext[i];
            }
        }

        printf("ciphertext: %s\n", cipher);
    }

}