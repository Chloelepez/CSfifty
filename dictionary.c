/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/
 
 
 
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

// link list item
struct node 
{
    char val [LENGTH+1];
    struct node* next;
};

// number of words in loaded dictionary, 0 if not loaded
uint32_t dictsize = 0;

// hash table
struct node* hashtable[HASHSIZE]; 

typedef struct pool
{ 
	int current_node_idx;
	struct node nodes[POOLSIZE];
}pool;

pool* pool_array[MAXPOOL];
int current_pool = -1;
 


//-------------------- actual code starts here ------------------------------



// Hash function MurmurHash3 was written by Austin Appleby, and is placed in 
// the public domain. I found my version on wikipedia.
 
uint32_t murmur3_32(const char *key, uint32_t len, uint32_t seed)
{
    static const uint32_t c1 = 0xcc9e2d51;
    static const uint32_t c2 = 0x1b873593;
    static const uint32_t r1 = 15;
    static const uint32_t r2 = 13;
    static const uint32_t m = 5;
    static const uint32_t n = 0xe6546b64;
    
    uint32_t hash = seed;

    const int nblocks = len / 4;
    const uint32_t *blocks = (const uint32_t *) key;
    int i;
    for (i = 0; i < nblocks; i++) 
    {
        uint32_t k = blocks[i];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;
        
        hash ^= k;
        hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
    }

    const uint8_t *tail = (const uint8_t *) (key + nblocks * 4);
    uint32_t k1 = 0;
    
    switch (len & 3) 
    {
        case 1:
        {
            k1 ^= tail[2] << 16;
            break;
        }
        
        case 2:
        {
            k1 ^= tail[1] << 8;
            break;
        }
        
        case 3:
        {
            k1 ^= tail[0];
            k1 *= c1;
            k1 = (k1 << r1) | (k1 >> (32 - r1));
            k1 *= c2;
            hash ^= k1;
            break;
        }
    }

    hash ^= len;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);

    return hash;
}

/**
 * Returns true if word is in dictionary else false.
 */
 
bool check(const char* word)
{
    // hash table index of word
    uint32_t hashkey; 
    
    // crawl through singly-linked list
    struct node* crawler; 
    
    // length of word
    int wordlen = strlen(word); 
    
    // store lower case of word
    char lowerword[wordlen + 1]; 

    // get hashkey
	 for (int i = 0; word[i]; i++) 
    {
        lowerword[i] = tolower(word[i]);   
	}
    lowerword[wordlen] = '\0';
    hashkey = murmur3_32(lowerword, (uint32_t) strlen(lowerword), HASHSEED) % HASHSIZE;
    
    // if hash bin is empty, return immediately
    if (!hashtable[hashkey])
    {
        return false;
    }

    else
    {
        // crawl through hash bin & check for word
        crawler = hashtable[hashkey];
        while (crawler)
        {
            if (strcmp(crawler->val,lowerword) == 0)
            {
                return true;
            }
            
            else
            {
                crawler = crawler->next;
            }
        }

        // if word is not found, return false
        return false;
    }
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // buffer to read dictionary words into (2 for \n and \0)
    char word[LENGTH + 2]; 
    
    // hash table index of word
    uint32_t hashkey; 
    
    // temporary node for construction
    struct node* temp; 

    // open dictionary
    FILE* dictfile = fopen(dictionary, "r");
    if (!dictfile)
    {
        printf("Error opening dictionary at %s.\n", dictionary);
        return false;
    }

    // clear hash table & dictionary size
    unload();
    dictsize = 0;

    // read word by word & enter into hash table
    while (fgets(word, sizeof(word) / sizeof(char), dictfile))
    {
        // trim new line character
        if (word[strlen(word) - 1] == '\n')
        {    
            word[strlen(word) - 1] = '\0';
        }
        // get hash key
        hashkey = murmur3_32(word, (uint32_t) strlen(word), HASHSEED) % HASHSIZE;
		
		// memory allocation by pools of nodes
		if (current_pool == -1) 
		{
			// create the first pool
			current_pool ++;
			pool_array[current_pool] = (pool *) malloc(sizeof(pool)*MAXPOOL);
			pool_array[current_pool]->current_node_idx = -1;
		}
		
		if (pool_array[current_pool]->current_node_idx == POOLSIZE)
		{
			// create the next pool
			if (current_pool == MAXPOOL)
			{
			// if too much words for maxpool
			   return false;
			   printf("too much words for the size of the pool");
			}
			current_pool++;
			pool_array[current_pool] = (pool *) malloc(sizeof(pool)*MAXPOOL);
			pool_array[current_pool]->current_node_idx = -1;
		}
		
		//next node in the pool
		pool_array[current_pool]->current_node_idx++;
		temp = &(pool_array[current_pool]->nodes[pool_array[current_pool]->current_node_idx]);
		
		// copy  word into the temporary node
        strncpy(temp->val, word, strlen(word));
        temp->val[strlen(word)] = '\0';
        
        // add to singly linked list 
        if (!hashtable[hashkey])
        {
            temp->next = NULL;
        }
        
        else
        {
            temp->next = hashtable[hashkey];
        }
        
        hashtable[hashkey] = temp;
        
        // count dictionary size
        dictsize++;
    }
    
    // close file  
    fclose(dictfile);

    // return true if successful
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
 
unsigned int size(void)
{
    return dictsize;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
 
bool unload(void)
{
    // free each pool
    for (int i = 0; i <= current_pool; i++)
    {
        free(pool_array[i]);  
                
    }

    // return true if successful
    return true;
}
