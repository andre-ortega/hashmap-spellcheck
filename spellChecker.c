/*
 * CS 261 Data Structures
 * Assignment 5
 * Name: Andre Ortega
 * Date: 3/5/2020
 */

#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int levenshtein(const char * s, const char * t);

/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE* file, HashMap* map)
{
    char * next = nextWord(file);

    while(next)
    {
        hashMapPut(map, next, 0);
        char * temp = next;
        next = nextWord(file);
        free(temp);
    }
}

/**
 * Checks the spelling of the word provded by the user. If the word is spelled incorrectly,
 * print the 5 closest words as determined by a metric like the Levenshtein distance.
 * Otherwise, indicate that the provded word is spelled correctly. Use dictionary.txt to
 * create the dictionary.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    HashMap* map = hashMapNew(1000);

    FILE* file = fopen("dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);

    char inputBuffer[256];
    int quit = 0;
    while (!quit)
    {
        printf("Enter a word or \"quit\" to quit: ");
        scanf("%s", inputBuffer);

        // Implement the spell checker code here..

        for (int i = 0 ; i < strlen(inputBuffer) ; i++)
        {
            inputBuffer[i] = tolower(inputBuffer[i]);
        } 

        if(hashMapContainsKey(map, inputBuffer))
        {
            if(strcmp(inputBuffer, "quit") != 0)
            {
                printf("\n%s is spelled\033[0;32m correctly\n\n\033[0m", inputBuffer);
            }
        }

        else
        {
            printf("\n%s is spelled\033[0;31m incorrectly \033[0m", inputBuffer);

            HashLink * current;

            char * suggestions[5];

            for(int i = 0 ; i < 5 ; i++)
            {
                suggestions[i] = malloc(128 * sizeof(char));
            }

            int array[5];
            for(int i = 0; i < 5 ; i++)
            {
                array[i] = 1000;
            }

            for(int i = 0; i < map->capacity ; i++)
            {
                current = map->table[i];

                while(current)
                {

                    int distance = levenshtein(current->key, inputBuffer);

                    int max_index = 0;

                    for(int j = 0; j < 5 ; j++)
                    {
                        if (array[j] > array[max_index])
                        {
                            max_index = j;
                        }
                    }

                    if(distance < array[max_index])
                    {
                        suggestions[max_index] = current->key;
                        array[max_index] = distance;
                    }

                    current = current->next;
                }
            }

            printf("\n\nSuggestions:\033[0;32m %s, %s, %s, %s, %s\n\n\033[0m", suggestions[0] , suggestions[1] , suggestions[2] , suggestions[3] , suggestions[4]);


            for(int i = 0 ; i < 5 ; i++)
            {
                free(suggestions[i]);
            }
        } 

        if (strcmp(inputBuffer, "quit") == 0)
        {
            quit = 1;
        } 
    }

    hashMapDelete(map);
    return 0;
}

int levenshtein(const char * s, const char * t)
{
    int len1 = strlen(s);
    int len2 = strlen(t);

    int matrix[len1 + 1][len2 + 1];
    int i;
    
    for(i = 0; i <= len1; i++)
    {
        matrix[i][0] = i;
    }

    for(i = 0; i <= len2; i++)
    {
        matrix[0][i] = i;
    }

    for(i = 1; i <= len1; i++)
    {
        int j;
        char c1;

        c1 = s[i - 1];
        for(j = 1; j <= len2; j++)
        {
            char c2;

            c2 = t[j - 1];

            if(c1 == c2)
            {
                matrix[i][j] = matrix[i - 1][j - 1];
            }

            else
            {
                int delete;
                int insert;
                int substitute;
                int minimum;

                delete = matrix[i - 1][j] + 1;
                insert = matrix[i][j - 1] + 1;
                substitute = matrix[i - 1][j - 1] + 1;
                minimum = delete;

                if(insert < minimum)
                {
                    minimum = insert;
                }

                if(substitute < minimum)
                {
                    minimum = substitute;
                }

                matrix[i][j] = minimum;
            }
        }
    }

    return matrix[len1][len2];

}
