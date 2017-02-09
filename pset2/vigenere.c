/*
  Kevin Perez
  7/1/16
  vigenere.c
  c_i = (p_i + k_j) % 26
*/
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int key_decipher(char key)
{
  if(isupper(key))
  {
    return key-65;
    
  }
  else
  {
    return key-97;
  }
}

char encrypt(char plaintext, char key)
{
  int shift = key_decipher(key);  
  if(isupper(plaintext)){
    int result = plaintext + shift;
    if(result > 90)
    {
      result = result - 90;
      plaintext = result + 64;
    }
    else
      plaintext = result;
  }
  else if(islower(plaintext))
  {
    int result = plaintext + shift;
    if(result > 122){
      result = result-122;
      plaintext = 96+result;
    }
    else
    {
      plaintext = result;
    }
  }
  
  return plaintext;
}

int main(int argc, char * argv[])
{
  // exit program if too many cmd arguments
  if(argc != 2)
  {
    printf("You fail\n");
    return 1;
  }
  
  string key = argv[1];
  for(int i = 0; i < strlen(key); ++i)
  {
    // exit program if cmd argument contains more than letters
    if(!isalpha(key[i])){
      printf("You fail.\n");
      return 1;
    }
  }
  
  string plaintext = GetString();
  int key_length = strlen(key);
  for(int i = 0, j = 0; i < strlen(plaintext); ++i)
  {
    if(isalpha(plaintext[i])){
      j = j % key_length;
      plaintext[i] = encrypt(plaintext[i], key[j]);
      ++j;
    }
  }
  
  printf("%s\n", plaintext);
  
  return 0;
}