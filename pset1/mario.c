#include <stdio.h>
int main()
{
    int input = 0,
        sresult = 0,
        temp = 0;
    printf("Height: ");
    if((temp = getc(stdin)))
    {
        if(temp == '\n')
        {
            sresult = 0;
            printf("Retry: ");   
        }
        else
            ungetc(temp, stdin);
    }
    
    sresult = scanf("%d", &input);
    while(!sresult || input < 0 || input > 23){
        if(sresult == 0){
            printf("Retry: ");
            while(getchar() != '\n'){}
            sresult = scanf("%d", &input);
        }
        else{
            printf("Height: ");
            sresult = scanf("%d", &input);
        }
    }
    
    
    for(int i = 0; i < input; ++i){
        for(int j = 0; j < input - i - 1; ++j)
            printf(" ");
    
        for(int k = 0; k < i + 2; ++k)
            printf("#");
        printf("\n");
    }
    
}