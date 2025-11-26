//
// Created by dan on 11/26/25.
//

#ifndef HASHMAP_H
#define HASHMAP_H
#include <stddef.h>

typedef union {
    void *v_ptr;
    int v_int;
    long v_long;
} HashMap_Value;

typedef struct {
    HashMap_Value value;
    long key;
    void *next;
} HashMap_Node;

typedef struct {
    size_t cap;
    size_t n_buckets_filled;
    size_t n_keys;
    HashMap_Node **buckets;
} HashMap;

/**
 * Creates a hashmap with capacity 2^capacity_power
 * @param capacity_power The power of 2 of the target capacity
 * @return pointer to new hashmap
 */
HashMap *HashMap_Create(unsigned capacity_power);

long HashMap_KeyFromString(const char *str);

/**
 * Get value by key from hashmap
 * @param map The pointer to the relevant hashmap
 * @param key The key
 * @param out_value The pointer where the result should be written to
 * @return 0 if key was found, -1 else
 */
int HashMap_Get(const HashMap *map, long key, HashMap_Value *out_value);

/**
 * Inserts a key value pair into a hashmap (modifies entry if key already exists)
 * @param map The pointer to the relevant hashmap
 * @param key The key
 * @param value The value to insert
 */
void HashMap_Put(HashMap *map, long key, HashMap_Value value);

void HashMap_PutInt(HashMap *map, long key, int value);

void HashMap_PutLong(HashMap *map, long key, long value);

void HashMap_PutPtr(HashMap *map, long key, void *value);

/**
 * Get value by key from hashmap
 * @param map The pointer to the relevant hashmap
 * @param key The string key
 * @param out_value The pointer where the result should be written to
 * @return 0 if key was found, -1 else
 */
int HashMap_StrGet(const HashMap *map, const char *key, HashMap_Value *out_value);

/**
 * Inserts a key value pair into a hashmap (modifies entry if key already exists)
 * @param map The pointer to the relevant hashmap
 * @param key The key
 * @param value The value to insert
 */
void HashMap_StrPut(HashMap *map, const char *key, HashMap_Value value);

void HashMap_StrPutInt(HashMap *map, const char *key, int value);

void HashMap_StrPutLong(HashMap *map, const char *key, long value);

void HashMap_StrPutPtr(HashMap *map, const char *key, void *value);

/**
 * Frees up memory allocated by HashMap_Create and Put actions
 * @param map Hashmap to free
 */
void HashMap_Destroy(HashMap *map);

#endif //HASHMAP_H
