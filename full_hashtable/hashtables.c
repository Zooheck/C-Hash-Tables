#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair
{
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable
{
  int capacity;
  LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL)
  {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
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

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  // allocate memory for hash table
  HashTable *table = malloc(sizeof(HashTable));
  // initialize storage buckets
  table->storage = calloc(capacity, sizeof(LinkedPair *));
  // initialize capacity
  table->capacity = capacity;
  return table;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *table, char *key, char *value)
{
  // hash the key to get the index
  int index = hash(key, table->capacity);
  if (table->storage[index] == NULL)
  {
    table->storage[index] = create_pair(key, value);
  }
  else
  {
    LinkedPair *item = table->storage[index];
    while (item->next != NULL)
    {
      if (strcmp(item->key, key) == 0)
      {
        item->value = value;
        break
      }
      item = item->next;
    }
    if (strcmp(item->key, key) == 0)
    {
      item->value = value;
    }
    else
    {
      item->next = create_pair(key, value);
    }
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *table, char *key)
{
  int index = hash(key, table->capacity);
  LinkedPair *item = table->storage[index];
  while (strcmp(item->key, key) == 0)
  {
    if (item->next == NULL)
    {
      destroy_pair(item);
      table->storage[index] = NULL;
      break;
    }
    else
    {
      LinkedPair *temp = item;
      item = item->next;
      destroy_pair(temp);
    }
  }

  if (table->storage[index])
  {
    while (item->next != NULL)
    {
      LinkedPair *nextItem = item->next;
      if (strcmp(nextItem->key, key) == 0)
      {
        item->next = nextItem->next;
        destroy_pair(nextItem);
      }
      else
      {
        item = item->next;
      }
    }
    if (strcmp(node->next->key, key) == 0)
    {
      destroy_pair(node->next);
      node->next = NULL;
    }
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *table, char *key)
{
  int i = hash(key, table->capacity);
  LinkedPair *current = table->storage[i];
  while (current)
  {
    if (strcmp(current->key, key) == 0)
    {
      return current->value;
    }
    current = current->next;
  }
  return NULL;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *table)
{
  for (int i = 0; i < table->capacity; i++)
  {
    LinkedPair *current = table->storage[i];
    LinkedPair *next;
    while (current)
    {
      next = current->next;
      destroy_pair(current);
      current = next;
    }
  }
  free(table->storage);
  free(ht);
}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht = create_hash_table(ht->capacity * 2);
  for (int i = 0; i < ht->capacity; i++)
  {
    LinkedPair *pair = ht->storage[i];
    while (pair)
    {
      hash_table_insert(new_ht, pair->key, pair->value);
      pair = pair->next;
    }
  }
  destroy_hash_table(ht);
  return new_ht;
}

#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
