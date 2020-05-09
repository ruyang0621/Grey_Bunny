#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>

int main(int argc, string argv [])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key.\n");
        return 1;
    }
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else
    {
        char letter[26];
        for (int i = 0; i < 26; i++)
        {
            letter[i] = argv[1][i];
        }

        for (int i = 0; i < 26; i++)
        {
            if (ispunct(letter[i]) || isdigit(letter[i]))
            {
                printf("Key must contain 26 characters.\n");
                return 1;
            }
            else
            {
                letter[i] = toupper(letter[i]);
                for (int j = i + 1; j < 26; j++)
                {
                    if (letter[i] == letter[j])
                    {
                        printf("Key must contain 26 characters.\n");
                        return 1;
                    }
                }
            }
        }

        string text = get_string("plaintext: ");
        printf("ciphertext: ");
        for (int i = 0; i < strlen(text); i++)
        {
            if (isupper(text[i]))
            {
                int a = text[i] - 'A';
                printf("%c", toupper(letter[a]));
            }
            else if (islower(text[i]))
            {
                int a = text[i] - 'a';
                printf("%c", tolower(letter[a]));
            }
            else
            {
                printf("%c", text[i]);
            }
        }
        printf("\n");
    }

}