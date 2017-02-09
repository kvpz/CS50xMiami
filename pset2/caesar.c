/*
  Kevin Perez
  caesar.c
  7/1/16

  c_i = (p_i + k) % 26
*/
#include <stdio.h>
#include <stdlib.h> /* atoi */
#include <cs50.h>
#include <string.h> /* strlen */
#include <ctype.h>

char encrypt(char p, int key){
  if(isalpha(p) && isupper(p)){
    int result = p + key;
    if(result > 90){
      result = result-90;
      p = 64+result;
    } 
    else{
      p = result;
    }
  }
  else if(isalpha(p) && islower(p)){
   int result = p + key;
   if(result > 122){
     result = result-122;
     p = 96+result;
   }
   else{
     p = result;
   }
  }
  return p;
}

int main(int argc, char ** argv)
{
  if(argc != 2){
    printf("You fail\n");
    return 1;
  }
  // it is assumed the user input is an integer
  int key = atoi(argv[1]);
  string p = GetString();
  key = key % 26; 
  
  for(int i = 0; i < strlen(p); ++i){
    p[i] = encrypt(p[i],key);
  }// for
  
  printf("%s\n", p);
  
}