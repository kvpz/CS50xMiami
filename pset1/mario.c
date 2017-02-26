#include <stdio.h>
#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

using namespace cgicc;

int main(int argc, char** argv)
{
    int input = 0,
        sresult = 0,
        temp = 0;
    if(argc > 1 && argc < 3)
    {
	input = atoi(argv[1]);
    }
    else
    {
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
    }
    
    for(int i = 0; i < input; ++i){
        for(int j = 0; j < input - i - 1; ++j)
            printf(" ");
    
        for(int k = 0; k < i + 2; ++k)
            printf("#");
        printf("\n");
    }
    
}
