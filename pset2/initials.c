/*
  Kevin Perez
  6/30/16
  initials.c
  
  Program returns the initials of a name. 
  It is assumed that each part of a person's full name is
  separated by a single space and can be upper/lowercase.
*/
#include <cs50.h> /* GetString() */
#include <stdio.h>
#include <string.h> /* strlen() */ 
#include <ctype.h> /* toupper()*/

char* get_initials(string name){
  static char initials[5]; // assuming no more than 5 initials
  initials[0] = toupper(name[0]); // First name initial
  int itr = 1;
  int name_length = strlen(name);
  
  // Storing rest of name initials in initials array
  for(int i = 1; i < name_length; ++i){
    if(name[i] == ' ' && i < name_length-1){
      initials[itr] = toupper(name[i+1]);
      ++itr;
    }
  }
  return initials;
}

int main(void)
{
  string name = GetString();
  printf("%s\n", get_initials(name));
}

