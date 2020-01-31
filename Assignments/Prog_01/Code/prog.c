#include <stdio.h>
int main(int argc, char *argv[])
{
    int a, b, lo, hi, count, isPrime;
    //to catch two inputs
    if (argc == 3)
    {
        sscanf(argv[1], "%d", &a);
        sscanf(argv[2], "%d", &b);
        //error handling
        if (a == b)
        {
            return printf("%s's argument's must be different values.\n", argv[0]);
        }
        else if (a < 0)
        {
            return printf("%s's first argument is not strictly postive interger.\n", argv[0]);
        }
        else if (b < 0)
        {
            return printf("%s's second argument is not strictly postive interger.\n", argv[0]);
        }
        //making sure the inputs are in order
        else if (a > b)
        {
            hi = a;
            lo = b;
        }
        else
        {
            hi = b;
            lo = a;
        }
        //beginning the prime list
        printf("The prime numbers between %d and %d are: ", lo, hi);
        while (lo < hi)
        {
            //flag used to tell if it is prime or not
            isPrime = 1;
            //loop that tries 2 to n - 1 to see if it can be divided into, breaks early if it can be to move on.
            for (int i = 2; i <= lo - 1; i++)
            {
                if (lo % i == 0)
                {
                    isPrime = 0;
                    break;
                }
            }
            //if it proves to be prime then we print it
            if (isPrime == 1)
            {
                //0 and 1 are neither prime nor composite.
                if (lo == 1 || lo == 0)
                {
                }
                else
                {
                    printf("%d", lo);
                    //comma or period if it is the last one
                    if (lo == hi - 1)
                    {
                        printf(".\n");
                    }
                    else
                    {
                        printf(", ");
                    }
                }
            }
            ++lo;
        }
    }
    //single input
    if (argc == 2)
    {
        sscanf(argv[1], "%d", &a);
        //error handling
        if (a < 0)
        {
            return printf("%s's argument is not a strictly positive integer.\n", argv[0]);
        }
        //quick short circuit so we don't run the while loop for just 1.
        else if (a == 1)
        {
            printf("The first prime number is 2.\n");
        }
        else
        {
            printf("The first %d prime numbers are: 2, ", a);
            //count starts at 2 because 1 (the number 2) is hard coded in.
            count = 2;
            //this uses a modified version of the previous loop but instead of asking for two numbers we always start
            //at three since we already know 0 and 1 aren't prime and 2 is a given.
            lo = 3;
            while (count <= a)
            {
                isPrime = 1;

                for (int i = 2; i <= lo - 1; ++i)
                {
                    if (lo % i == 0)
                    {
                        isPrime = 0;
                        break;
                    }
                }
                if (isPrime == 1)
                {
                    printf("%d", lo);
                    //slight modification from pervious version
                    if (count == a)
                    {
                        printf(".\n");
                    }
                    else
                    {
                        printf(", ");
                    }
                    //counter to make sure we go until a
                    count++;
                }
                ++lo;
            }
        }
    }
    return 0;
}
