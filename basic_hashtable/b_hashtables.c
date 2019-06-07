#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****
  Basic hash table key/value pair
 ****/
typedef struct Pair
{
  char *key;
  char *value;
} Pair;

/****
  Basic hash table
 ****/
typedef struct BasicHashTable
{
  int capacity;
  Pair **storage;
} BasicHashTable;

/****
  Create a key/value pair to be stored in the hash table.
 ****/
Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
  pair->key = strdup(key);
  pair->value = strdup(value);

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(Pair *pair)
{
  if (pair != NULL)
  {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/****
  djb2 hash function

  Do not modify this!
 ****/
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char *u_str = (unsigned char *)str;

  while ((c = *u_str++))
  {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/****
  Fill this in.

  All values in storage should be initialized to NULL
  (hint: look up `calloc`)
 ****/
BasicHashTable *create_hash_table(int capacity)
{
  // allocate memory for the hash table skeleton
  BasicHashTable *table = malloc(sizeof(BasicHashTable));
  // initialize storage buckets; each bucket contains NULL value
  table->storage = calloc(capacity, sizeof(Pair *));
  table->capacity = capacity;
  return table;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *table, char *key, char *value)
{
  // hash the key passed in to the function
  int index = hash(key, table->capacity);
  // if there is a value at this index...
  if (table->storage[index] != NULL)
  {
    // destroy the key-value pair
    destroy_pair(table->storage[index]);
    // add the new key-value pair in the newly empty place
    table->storage[index] = create_pair(key, value);
  }
  else // AKA: The bucket is unoccupied and we can fill it in
  {
    table->storage[index] = create_pair(key, value);
  }
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *table, char *key)
{
  int index = hash(key, 16);
  destroy_pair(table->storage[index]);
  table->storage[index] = NULL;
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *table, char *key)
{
  int index = hash(key, 16);
  if (table->storage[index] != NULL)
  {
    return table->storage[index];
  }
  else
  {
    return NULL;
  }
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *table)
{
  for (int i = 0; i < table->capacity - 1; i++)
  {
    if (table->storage[i])
    {
      destroy_pair(table->storage[i]);
    }
  }
  free(table->storage);
  free(table);
}

#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);

  hash_table_insert(ht, "line", "Here today...\n");

  printf("%s", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");

  if (hash_table_retrieve(ht, "line") == NULL)
  {
    printf("...gone tomorrow. (success)\n");
  }
  else
  {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}
#endif
