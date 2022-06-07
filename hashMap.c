/*
 * CS 261 Data Structures
 * Assignment 5
 * Name: Andre Ortega
 * Date: 3/5/2020
 */

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int hashFunction1(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += key[i]; //simple add
    }
    return r;
}

int hashFunction2(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += (i + 1) * key[i]; //mult w/ shift
    }
    return r;
}

/**
 * Creates a new hash table link with a copy of the key string.
 * @param key Key string to copy in the link.
 * @param value Value to set in the link.
 * @param next Pointer to set as the link's next.
 * @return Hash table link allocated on the heap.
 */
HashLink* hashLinkNew(const char* key, int value, HashLink* next)
{
    HashLink* link = malloc(sizeof(HashLink));
    link->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(link->key, key);
    link->value = value;
    link->next = next;
    return link;
}

/**
 * Free the allocated memory for a hash table link created with hashLinkNew.
 * @param link
 */
static void hashLinkDelete(HashLink* link)
{
    free(link->key);
    free(link);
}

/**
 * Initializes a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param map
 * @param capacity The number of table buckets.
 */
void hashMapInit(HashMap* map, int capacity)
{
    map->capacity = capacity;
    map->size = 0;
    map->table = (HashLink **)malloc(sizeof(HashLink*) * capacity);
    for (int i = 0; i < capacity; i++)
    {
        map->table[i] = NULL;
    }
}

/**
 * Removes all links in the map and frees all allocated memory. You can use
 * hashLinkDelete to free the links.
 * @param map
 */
void hashMapCleanUp(HashMap* map)
{
    HashLink * current;
    HashLink * garbage;

    for(int i = 0 ; i < map->capacity ; i++)
    {
        current = map->table[i];

        while(current)
        {
            garbage = current;
            current = current->next;
            hashLinkDelete(garbage);
        }

    }

    free(map->table);
    map->size = 0;
}

/**
 * Creates a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param capacity The number of buckets.
 * @return The allocated map.
 */
HashMap* hashMapNew(int capacity)
{
    HashMap* map = malloc(sizeof(HashMap));
    hashMapInit(map, capacity);
    return map;
}

/**
 * Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * @param map
 */
void hashMapDelete(HashMap* map)
{

    hashMapCleanUp(map);
    free(map);
    map = NULL;

}

/**
 * Returns a pointer to the value of the link with the given key  and skip traversing as well. Returns NULL
 * if no link with that key is in the table.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return Link value or NULL if no matching link.
 */
int* hashMapGet(HashMap* map, const char* key)
{
    

    int index = abs(HASH_FUNCTION(key) % map->capacity);

    HashLink * iter = map->table[index];

    while(iter)
    {
        if(strcmp(iter->key, key) == 0)
        {
            return &(iter->value);
        }

        else
        {
            iter = iter->next;
        }
    }

    return NULL;
}

/**
 * Resizes the hash table to have a number of buckets equal to the given 
 * capacity (double of the old capacity). After allocating the new table, 
 * all of the links need to rehashed into it because the capacity has changed.
 * 
 * Remember to free the old table and any old links if you use hashMapPut to
 * rehash them.
 * 
 * @param map
 * @param capacity The new number of buckets.
 */
void resizeTable(HashMap* map, int capacity)
{
    HashLink * current;
    HashMap * newMap = hashMapNew(capacity * 2);

                /*
                HashMap* hashMapNew(int capacity)
                {
                    printf("\nmark 0.5 New Map");
                    HashMap* map = malloc(sizeof(HashMap));
                    hashMapInit(map, capacity);
                    printf("\nmark 0.55 End New Map");
                    return map;
                }*/
    
    for(int i = 0 ; i < map->capacity ; i++)
    {
        current = map->table[i];

        while(current)
        {
            hashMapPut(newMap, current->key, current->value);
            current = current->next;
        }
    }
    

    hashMapCleanUp(map);

    map->size = newMap->size;
    map->table = newMap->table;
    map->capacity = newMap->capacity;

    free(newMap);
    
}

/**
 * Updates the given key-value pair in the hash table. If a link with the given
 * key already exists, this will just update the value and skip traversing. Otherwise, it will
 * create a new link with the given key and value and add it to the table
 * bucket's linked list. You can use hashLinkNew to create the link.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket.
 * 
 * @param map
 * @param key
 * @param value
 */
void hashMapPut(HashMap* map, const char* key, int value)
{

    if(hashMapContainsKey(map, key))
    {
        *hashMapGet(map, key) = value;
    }

    else
    {
        int index = abs(HASH_FUNCTION(key) % map->capacity);


        map->table[index] = hashLinkNew(key, value, map->table[index]);

        map->size++;
        
        float factor = map->size / map->capacity;

        if(factor > 0.75)
        {
            resizeTable(map, map->capacity);
        }
    }

        /*

        HashLink* hashLinkNew(const char* key, int value, HashLink* next)
        {
            HashLink* link = malloc(sizeof(HashLink));
            link->key = malloc(sizeof(char) * (strlen(key) + 1));
            strcpy(link->key, key);
            link->value = value;
            link->next = next;
            return link;
        } */
}

/**
 * Removes and frees the link with the given key from the table. If no such link
 * exists, this does nothing. Remember to search the entire linked list at the
 * bucket. You can use hashLinkDelete to free the link.
 * @param map
 * @param key
 */
void hashMapRemove(HashMap* map, const char* key)
{

  int index = abs(HASH_FUNCTION(key) % map->capacity);

  HashLink * previous = NULL; 

  HashLink * current = map->table[index];
  
  while(current)
  {

      if(strcmp(current->key, key) == 0)
      {

          if(previous == NULL)
          {
              map->table[index] = previous;
          }

          else
          {
              previous->next = current->next;
          }
          
          hashLinkDelete(current);
          map->size--;
      }

      previous = current;
      current = current->next;
  }
}


/**
 * Returns 1 if a link with the given key is in the table and 0 otherwise.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return 1 if the key is found, 0 otherwise.
 */
int hashMapContainsKey(HashMap* map, const char* key)
{ 

    if(hashMapGet(map, key))
    {
        return 1;
    }

    else
    {
        return 0;
    }
}

/**
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(HashMap* map)
{
    return map->size;
}

/**
 * Returns the number of buckets in the table.
 * @param map
 * @return Number of buckets in the table.
 */
int hashMapCapacity(HashMap* map)
{
    return map->capacity;
}

/**
 * Returns the number of table buckets without any links.
 * @param map
 * @return Number of empty buckets.
 */
int hashMapEmptyBuckets(HashMap* map)
{
    int empty = 0;
    for(int i = 0 ; i < map->capacity ; i++)
    {
        if(map->table[i] == NULL)
        {
            empty++;
        }
    }

    return empty;
}

/**
 * Returns the ratio of (number of links) / (number of buckets) in the table.
 * Remember that the buckets are linked lists, so this ratio tells you nothing
 * about the number of empty buckets. Remember also that the load is a floating
 * point number, so don't do integer division.
 * @param map
 * @return Table load.
 */
float hashMapTableLoad(HashMap* map)
{
    float factor = map->size / map->capacity;
    return factor;
}

/**
 * Prints all the links in each of the buckets in the table.
 * @param map
 */
void hashMapPrint(HashMap* map)
{

    HashLink * link;

    for(int i = 0 ; i < map->capacity ; i++)
    {
        printf("\n%d: ", i);
        
        link = map->table[i];

        while(link)
        {
            printf("key: %s, value: %d --> ", link->key, link->value);
            link = link->next;
        }

        printf("(null)\n");
    } 

}
