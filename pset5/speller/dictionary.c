// Implements a dictionary's functionality
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> 
#include <strings.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//Number of words in dictionary
unsigned long word_count = 0;

// Number of buckets in hash table
const unsigned int N = 16384;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //covert all characters in word to lowercase.
    char copy[strlen(word) + 1];
    strcpy(copy, word);
    for (int i = 0; i < strlen(word) + 1; i++)
    {
        copy[i] = tolower(copy[i]);
    }
    
    //hash the word
    unsigned int hash_val = hash(copy);
    
    //traverse linked list, looking for the word
    node *cursor = table[hash_val];
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, copy) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
// Use Hash Function Times 33 with Addition which is from Daniel J. Bernstein
unsigned int hash(const char *word)
{
    unsigned long h = 5381;
    int c;

    while ((c = *word++))
    {
        h = ((h << 5) + h) + c; /* hash * 33 + c */
    }

    return h % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    char buffer[LENGTH];
    //read strings from file one at a time
    while (fscanf(file, "%s", buffer) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return 1;
        }
        strcpy(n->word, buffer);
        n->next = NULL;

        //using hash function convert string to index
        unsigned int hash_value = hash(buffer);
        if (table[hash_value] != NULL)
        {
            n->next = table[hash_value];
        }
        table[hash_value] = n;
        word_count++;
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
