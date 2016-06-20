#include <stdio.h>
int main(void)
{
    int minutes,
        read = 0,
        temp = 0;;
    printf("minutes: ");
    
    // The solution for rejecting "" minutes
    if((temp = getc(stdin)))
    {
        if(temp == '\n')
        {
            read = 0;
            printf("Retry: ");   
        }
        else
            ungetc(temp, stdin);
    }
        
    while(read == 0)
    {
        read = scanf("%d", &minutes); // best to use %u format specifier
        if(read && minutes > 0)
        {
            if(getchar() != '\n')
            {
                read = 0;
            }
        }
        else
        {
            printf("Retry: ");
            while(getchar() != '\n'){}
            read = 0;
        }
    }
    printf("bottles: %d\n", minutes*12);
}