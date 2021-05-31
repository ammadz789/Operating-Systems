#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fptr;
    char ch ;
    int c;
    int counter = 0;
    char filename[25] = "loremipsum.txt";

    fptr = fopen(filename, "r");

    if (fptr != NULL)
    {
        while((c = getc(fptr)) != EOF)
        {
            if ('a' == c)
            {
                counter++;
            }
        }
    }
    else
    {
        printf("Cannot open file! \n");
    }

    char a = 'a';
    printf("Number of times '%c' occurred = %d\n", a, counter);

    fclose(fptr);
    return 0;
}
