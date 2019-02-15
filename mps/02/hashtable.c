#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

/* Daniel J. Bernstein's "times 33" string hash function, from comp.lang.C;
   See https://groups.google.com/forum/#!topic/comp.lang.c/lSKWXiuNOAk */
unsigned long hash(char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

hashtable_t *make_hashtable(unsigned long size) {
  hashtable_t *ht = malloc(sizeof(hashtable_t));
  ht->size = size;
  ht->buckets = calloc(sizeof(bucket_t *), size);
  return ht;
}

void ht_put(hashtable_t *ht, char *key, void *val) {
  /* FIXME: the current implementation doesn't update existing entries */
  unsigned long idx = hash(key) % ht->size;
  bucket_t *b = ht->buckets[idx];
  while (b) {
    if (strcmp(key, b->key) == 0) {
      free(b->val);
      b->val = val;
      free(key);
      return;
    }
    b = b->next;
  }
  b = malloc(sizeof(bucket_t));
  b->key = key;
  b->val = val;
  b->next = ht->buckets[idx];
  ht->buckets[idx] = b;
}

void *ht_get(hashtable_t *ht, char *key) {
  unsigned long idx = hash(key) % ht->size;
  bucket_t *b = ht->buckets[idx];
  while (b) {
    if (strcmp(b->key, key) == 0) {
      return b->val;
    }
    b = b->next;
  }
  return NULL;
}

void ht_iter(hashtable_t *ht, int (*f)(char *, void *)) {
  bucket_t *b;
  unsigned long i;
  for (i=0; i<ht->size; i++) {
    b = ht->buckets[i];
    while (b) {
      if (!f(b->key, b->val)) {
        return ; // abort iteration
      }
      b = b->next;
    }
  }
}

void free_hashtable(hashtable_t *ht) {
  bucket_t *b, *bucket_to_free;
  unsigned long i;
  for (i=0; i<ht->size; i++) {
    b = ht->buckets[i];
    while (b) {
      bucket_to_free = b;
      b = b->next;
      free(bucket_to_free->key);
      free(bucket_to_free->val);
      free(bucket_to_free);
    }
  }
  free(ht->buckets);
  free(ht); // FIXME: must free all substructures!
}

/* TODO */
void  ht_del(hashtable_t *ht, char *key) {
  unsigned long idx = hash(key) % ht->size;
  bucket_t *curr = ht->buckets[idx], *prev; 

  if (curr != NULL && strcmp(curr->key, key) == 0) { 
      ht->buckets[idx] = curr->next;
      free(curr->key);
      free(curr->val);
      free(curr);
      return; 
  }
  while (curr != NULL && strcmp(curr->key, key) != 0) 
  { 
      prev = curr; 
      curr = curr->next; 
  }

  if (curr == NULL)
    return;

  prev->next = curr->next;

  free(curr->key);
  free(curr->val);
  free(curr);
}

void  ht_rehash(hashtable_t *ht, unsigned long newsize) {
  unsigned long oldsize = ht->size;
  bucket_t **oldbuckets = ht->buckets;
  ht->buckets = calloc(sizeof(bucket_t *), newsize);
  ht->size = newsize;
  for (unsigned long i=0; i<oldsize; i++) {
    bucket_t *b = oldbuckets[i];
    while (b) {
      ht_put(ht, b->key, b->val);
      b = b->next;
    }
    free(oldbuckets[i]);
  }
  free(oldbuckets);
}
