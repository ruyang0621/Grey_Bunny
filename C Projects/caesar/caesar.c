#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    if (argc == 1)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else
    {
        for (int j = 0; j < strlen(argv[1]); j++)
        {
            if (isalpha(argv[1][j]))
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }
    
        int key = atoi(argv[1]);
        if (argc != 2 || key < 0)
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
        else
        {
            string text = get_string("plaintext: ");
            printf("ciphertext: ");
            for (int i = 0; i < strlen(text); i++)
            {
                if (ispunct(text[i]) || isblank(text[i]))
                {
                    printf("%c", text[i]);
                }
                else if (isupper(text[i]))
                {
                    int p1 = (int) text[i] - 65;
                    int c1 = (p1 + key) % 26;
                    printf("%c", (char) c1 + 65);
                }
                else if (islower(text[i]))
                {
                    int p2 = (int) text[i] - 97;
                    int c2 = (p2 + key) % 26;
                    printf("%c", (char) c2 + 97);
                }
            }
            printf("\n");
            return 0;
        }
    }

    
}