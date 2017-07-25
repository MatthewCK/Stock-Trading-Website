/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>


#include "dictionary.h"

typedef struct node 
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

node *hashtable[hash_num];

int dict_size = 0;
bool is_loaded = false;

// hash function by Dan Bernstein
unsigned long
hash(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // TODO
    
    int str = strlen(word);
        
    char lower_word[str + 1];
    
    for (int i = 0; i < str; i++)
    {
        lower_word[i] = tolower(word[i]);
    }
    
    lower_word[str] = '\0';

    // get hash for word
    int h = hash(lower_word) % hash_num;

    node *cursor = hashtable[h];
    
    // compare words and move cursor if no match.
    while (cursor != NULL)
    {
        if (strcasecmp(lower_word, cursor->word) == 0)
            return true;
        else
            cursor = cursor->next;
    }
    
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
 
 
bool load(const char *dictionary)
{
    // TODO hash table to linked lists
    FILE *file;
    
    file = fopen(dictionary, "r");
    
    char word_t[LENGTH + 1];
    
    while(fscanf(file, "%s", word_t) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }
        
        dict_size++;
        
        
        strcpy(new_node->word, word_t);
        
        unsigned long index = hash(new_node->word) % 50;
        
        new_node->next = hashtable[index];
        hashtable[index] = new_node;
    }
    
    is_loaded = true;
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (is_loaded == true)
        return dict_size;
    else
        return 0;
        //return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    for (int i = 0; i < hash_num; i++)
    {
        node *cursor = hashtable[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            
            free(temp);
        }
    }
    
    is_loaded = false;
    return true;
}
