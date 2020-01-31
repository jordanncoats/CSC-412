#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct data
{
    char name[8];
    unsigned int index;
    unsigned int rangeCount;
    unsigned int startRange[10];
    unsigned int endRange[10];
};
struct cat
{
    unsigned int count;
    struct data type[10];
};

int cmpfunc(const void *a, const void *b)
{
    if (*(char *)a > *(char *)b)
        return 1;
    return 0;
}

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

void orderSort(char *str, char *outStr, struct cat cats)
{
    int n = strlen(str);
    int m = 0;
    qsort(str, (size_t)strlen(str), (size_t)sizeof(char), cmpfunc);
    //Structs enable one set of nested for loops for each category instead of creating a new one with hardcoded values.
    for (int i = 0; i < cats.count; i++)
    {
        for (int k = 0; k < cats.type[i].rangeCount; k++)
        {
            for (int j = 0; j < n; j++)
            {
                if (str[j] >= cats.type[i].startRange[k] && str[j] <= cats.type[i].endRange[k])
                {
                    outStr[m] = str[j];
                    str[j] = 0;
                    m++;
                }
            }
        }
    }
    //to get any characters left behinds
    for (int l = 0; l < n; l++)
    {
        if (str[l] != 0)
        {

            outStr[m] = str[l];
            m++;
        }
    }
}

int main()
{
    //populating the structs
    struct cat cats;
    cats.count = 3;
    strcpy(cats.type[2].name, "letters");
    cats.type[2].index = 2;
    cats.type[2].rangeCount = 2;
    cats.type[2].startRange[0] = 'A';
    cats.type[2].endRange[0] = 'Z';
    cats.type[2].startRange[1] = 'a';
    cats.type[2].endRange[1] = 'z';
    strcpy(cats.type[0].name, "numbers");
    cats.type[0].index = 0;
    cats.type[0].rangeCount = 1;
    cats.type[0].startRange[0] = '0';
    cats.type[0].endRange[0] = '9';
    strcpy(cats.type[1].name, "maths");
    cats.type[1].index = 1;
    cats.type[1].rangeCount = 7;
    cats.type[1].startRange[0] = '+';
    cats.type[1].endRange[0] = '+';
    cats.type[1].startRange[1] = '-';
    cats.type[1].endRange[1] = '-';
    cats.type[1].startRange[2] = '*';
    cats.type[1].endRange[2] = '*';
    cats.type[1].startRange[3] = '/';
    cats.type[1].endRange[3] = '/';
    cats.type[1].startRange[4] = '=';
    cats.type[1].endRange[4] = '=';
    cats.type[1].startRange[5] = '!';
    cats.type[1].endRange[5] = '!';
    cats.type[1].startRange[6] = '%';
    cats.type[1].endRange[6] = '%';

    while (1)
    {
        //User input up to 99 chars
        char *str = (char *)calloc(100, sizeof(char));
        char *outStr = (char *)calloc(100, sizeof(char));
        printf("enter an input string: ");
        scanf("%99[ -~ | \t]", str);
        getchar();

        //Checks for EXIT
        if (exitCheck(str) == 0)
            break;

        removeDups(str);

        orderSort(str, outStr, cats);

        printf("\"%s\"\n", outStr);

        //Clears the arrays for the next loop
        free(str);
        free(outStr);
    }
    exit(0);
}