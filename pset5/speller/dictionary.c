/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>

#include "dictionary.h"

//Define the size of the hashtable. 32768 is 2^15
#define HASHTABLE_SIZE 32768

int wordcount;

//Define the type of node and the information it contains
typedef struct node
{
  char word[LENGTH+1];
  struct node* next;
}
node;

node*hashtable[HASHTABLE_SIZE];

/*
Hash function was found on r/cs50,
as suggested by one of the short videos from the edx course page.
I modified it to make it easier for me to understand.
*/
int hash(char* word){
  unsigned int hash = 0;
  for(int i =0; i<strlen(word); i++){
    hash = (hash<<2)^word[i];
  }

  return hash % HASHTABLE_SIZE;
}


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    char tempWord[LENGTH+1];
    for(int i = 0; i< strlen(word); i++){
      tempWord[i] = tolower(word[i]);
    }
    tempWord[strlen(word)] = '\0';

    int index = hash(tempWord);

    if(hashtable[index]==NULL){
      return false;
    }

    else{
      node* currentWord = hashtable[index];
      while(currentWord != NULL){
        if(strcmp(tempWord, currentWord->word)==0){
          return true;
        }
        currentWord = currentWord->next;
      }
    }

    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{

    char word[LENGTH+1];

    //open dictionary file
    FILE* dictFile = fopen(dictionary, "r");

    if(dictFile == NULL){
      return false;
    }

    while(fscanf(dictFile, "%s\n", word) !=EOF){

      wordcount++;

      //Make a new node, give it the value of the word using string copy
      node* newWord = (malloc(sizeof(node)));
      strcpy(newWord->word, word);
      newWord->next = NULL;

      // Get the index of the word for the hashtable
      int index = hash(word);


      //Best case scenario, if hashtable at index is empty we can simply insert the word
      if(hashtable[index]==NULL){
        hashtable[index] = newWord;
      }

      else{
        //Make a node for the word at the index
        node* currentWord = hashtable[index];
        //Loop through all the nodes at the index until we get to the last one
        while(currentWord->next != NULL){
          currentWord = currentWord->next;
        }
        //The node after the last one at the index is now the new word
        currentWord->next = newWord;

      }


    }

    fclose(dictFile);
    return true;




}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return wordcount;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    //Loops through every index possible for the hashtable
    for(int i = 0; i<HASHTABLE_SIZE; i++){
        
        //Checks if the current node is not null
        while(hashtable[i]!=NULL){
            //Frees the current node and makes the current node the next node in the linked list
            node* currentWord = hashtable[i];
            hashtable[i]=currentWord->next;
            free(currentWord);
        }
    }
    
    return true;
}
