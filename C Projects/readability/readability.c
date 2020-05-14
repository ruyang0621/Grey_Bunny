#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int index(float letters, float words, float sentences);

int main(void)
{
    string t = get_string("Text: ");

    int a = 0;
    int b = 0;
    int c = 0;
    for (int i = 0, n = strlen(t); i < n; i++)
    {
        if (isalpha(t[i]))
        {
            a++;
        }

        if (isspace(t[i]) && (isalpha(t[i + 1]) || ispunct(t[i + 1])))
        {
            b++;
        }

        if ((int) t[i] == 46 || (int) t[i] == 33 || (int) t[i] == 63)
        {
            c++;
        }
    }

    if (index(a, b + 1, c) >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index(a, b + 1, c) < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index(a, b + 1, c));
    }

}

int index(float letters, float words, float sentences)
{
    float l = letters / words * 100;
    float s = sentences / words * 100;
    return round(0.0588 * l - 0.296 * s - 15.8);
}