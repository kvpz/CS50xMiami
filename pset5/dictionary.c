/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 * 
 * words from dictionaries/large will only be a max of 45 characters
 * long and there will be about 143,091 words. Each is '\n' terminated.
 */
#include <stdio.h>
#include <stdbool.h>

#include "dictionary.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // tolower()

unsigned int wordsInDictionary = 0; // words counted upon loading file

struct Node
{
    bool isEndOfString: 1;
    char data;
    struct Node *left, 
                *mid, 
                *right;
};
    
struct Node* newNode(char data)
{
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    temp->data = data;
    temp->isEndOfString = 0;
    temp->left = temp->mid = temp->right = NULL;
    return temp;
}

void insert(struct Node** root, char * word)
{
    // if tree is empty
    if(!(*root))
        *root = newNode(*word);
    
    // if current character < root, insert in root's left tree    
    if((*word) < (*root)->data)
    {
        insert(&((*root)->left), word);
    }
    // if current character > root character, insert in right tree
    else if((*word) > (*root)->data)
    {
        insert(&((*root)->right), word);
    }
    // if current character == root character, insert in middle tree
    else
    {
        if(*(word+1))
            insert(&((*root)->mid), word+1);
        else 
            (*root)->isEndOfString = 1;
    }
}

void traverseTSTUtil(struct Node* root, char* buffer, int depth)
{
    if(root)
    {
        traverseTSTUtil(root->left, buffer, depth);
        
        buffer[depth] = root->data;
        if(root->isEndOfString)
        {
            buffer[depth+1] = '\0';
            printf("%s\n", buffer);
        }
        
        traverseTSTUtil(root->mid, buffer, depth + 1);
        
        traverseTSTUtil(root->right, buffer, depth);
    }
}

// search tree for word
int searchTST(struct Node * root, const char * word)
{
    if(!root)
    {
        return 0;
    }
    
    if(*word < (root)->data)
        return searchTST(root->left, word);
    else if(*word > (root)->data)
        return searchTST(root->right, word);
    else
    {
        if(*(word+1) == '\0') // =='\0'
            return root->isEndOfString;
        
        return searchTST(root->mid, word+1);
    }
}

struct Node * root = NULL;

/**
 * Returns true if word is in dictionary else false.
 * This check is case-insensitive. Only alphabetical characters
 * and apostrophes may be passed into check.
 */
bool check(const char* word)
{
    int itr = 0;
    char str[strlen(word)+1];
    while(word[itr] != '\0')
    {
        str[itr] = tolower(word[itr]);
        ++itr;
    }
    str[itr] = '\0';
    return searchTST(root, str);
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    
    FILE * fin; // = fopen(dictionary, "w");
    if(!(fin = fopen(dictionary, "r")))
        return false;
    char c;
    int i = 0;
    char word[46];
    while((c = fgetc(fin)) != EOF)
    {
        if(c == '\n') // end of word
        {
            ++wordsInDictionary;
            char str[i];
            strncpy(str, word, i);
            str[i] = '\0';
            insert(&root, str);
            i = 0;
        }
        else
            word[i++] = c;
    }
    fclose(fin);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return wordsInDictionary;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    free(root);
    root = NULL;
    if(!root)
        return true;
    else
        return false;
}
