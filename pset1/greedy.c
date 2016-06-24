/*
    greedy.c
    Kevin Perez
    6/23/16

    After the program prompts the user how much change is owed, it will 
    output the minimum amount of coins with which said change can be made.
    It is assumed that the only coins available are the cents: 25,10,5, & 1.
*/
#include <stdio.h>
#include <cs50.h>
#include <math.h>

int minCoinsForChange(int cents){
    int coins = 0,
        remaining = 0;
    coins = cents/25;
    remaining = cents%25;
    coins += remaining/10;
    remaining = remaining%10;
    coins += remaining/5;
    remaining = remaining%5;
    coins += remaining;

    return coins;
}

int convertToCents(float change){
    int cents = 0;
    int temp = (int)(change*100);
    if(change > 1.0 && change > roundf(change)){
        cents = (roundf(change)*100.0);
        cents += temp%cents + 1;
    }
    else{
        cents = change*100;
    }
    return cents;
}

int main(void)
{
    printf("O hai! How much change is owed?\n");
    float change;
    
    do{
        change = GetFloat();
        if(change < 0)
            printf("How much change is owed?\n");
    }while(change < 0);
    
    int cents = convertToCents(change);
    printf("%d\n", minCoinsForChange(cents));
}