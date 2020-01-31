#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int exitCheck(char *str)
{
    //Short function to check if the string is EXIT
    char check[5] = "EXIT";
    for (int i = 0; i < 5; i++)
    {
        if (str[i] != check[i])
        {
            return 1;
        }
    }
    return 0;
}
//qsort compares by ascii
int cmpfunc(const void *a, const void *b)
{
    if (*(char *)a > *(char *)b)
        return 1;
    return 0;
}

void removeDups(char *str)
{
    //Loops through to find alike characters then moves the character after into the dup's place and so on
    for (int i = 0; i < strlen(str); i++)
    {
        for (int j = i + 1; str[j] != '\0'; j++)
        {
            if (str[j] == str[i])
            {
                for (int k = j; str[k] != '\0'; k++)
                {
                    str[k] = str[k + 1];
                }
            }
        }
    }
}

char orderSort(char *str, char *outStr)
{
    int n = strlen(str);
    int letterIndex = 0;
    int numberIndex = 0;
    int otherIndex = 0;
    //counts each type for sorting
    for (int i = 0; i < n; i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            otherIndex++;
        }
        else if ((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z'))
        {
            numberIndex++;
            otherIndex++;
        }
    }
    //moves them into a new list that is partially sorted by A/0/~
    for (int i = 0; i < n; i++)
    {
        if ((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z'))
        {
            outStr[letterIndex] = str[i];
            letterIndex++;
        }
        else if (str[i] >= '0' && str[i] <= '9')
        {
            outStr[numberIndex] = str[i];
            numberIndex++;
        }
        else
        {
            outStr[otherIndex] = str[i];
            otherIndex++;
        }
    }
}

int main()
{
    while (1)
    {
        char *str = (char *)calloc(100, sizeof(char));
        char *outStr = (char *)calloc(100, sizeof(char));
        //User input up to 99 chars
        printf("enter an input string: ");
        scanf("%99[ -~ | \t]", str);
        getchar();

        //Checks for EXIT
        if (exitCheck(str) == 0)
            break;

        removeDups(str);

        qsort(str, (size_t)strlen(str), (size_t)sizeof(char), cmpfunc);

        orderSort(str, outStr);

        printf("\"%s\"\n", outStr);

        //Clears the arrays for the next loop
        free(str);
        free(outStr);
    }
    exit(0);
}