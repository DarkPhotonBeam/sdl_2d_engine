//
// Created by dan on 11/26/25.
//

#include "../include/2DEngine/hashmap.h"

#include <SDL3/SDL_stdinc.h>

#define STR_HASH_PRIME 31
#define STR_MAX_LEN 128

size_t hash(const long key) {
    return key ^ (key >> (sizeof(long) * 4));
}

HashMap *HashMap_Create(const unsigned capacity_power) {
    if (capacity_power <= 0) {
        return nullptr;
    }
    HashMap *map = SDL_malloc(sizeof(HashMap));
    map->cap = 1 << capacity_power;
    map->n_buckets_filled = 0;
    map->n_keys = 0;
    map->buckets = SDL_malloc(map->cap * sizeof(HashMap_Node *));
    for (size_t i = 0; i < map->cap; i++) {
        map->buckets[i] = nullptr;
    }
    return map;
}

long HashMap_KeyFromString(const char *str) {
    long key = 0;
    long pow = 1;
    for (size_t i = 0; i < STR_MAX_LEN; i++) {
        const char c = str[i];
        if (c == '\0') break;
        key += pow * c;
        pow *= STR_HASH_PRIME;
    }
    return key;
}

int HashMap_Get(const HashMap *map, const long key, HashMap_Value *out_value) {
    const size_t index = hash(key) & (map->cap - 1);
    for (const HashMap_Node *curr = map->buckets[index]; curr != nullptr; curr = curr->next) {
        if (curr->key == key) {
            *out_value = curr->value;
            return 0;
        }
    }
    return -1;
}

void HashMap_Put(HashMap *map, const long key, const HashMap_Value value) {
    const size_t index = hash(key) & (map->cap - 1);
    bool key_exists = false;
    HashMap_Node *new_node = nullptr;
    HashMap_Node *last = map->buckets[index];
    if (last != nullptr) {
        while (last != nullptr) {
            if (last->key == key) {
                new_node = last;
                key_exists = true;
                // break since node for key already exists
                break;
            }
            if (last->next == nullptr) break;
            last = last->next;
        }
    }
    if (new_node == nullptr) {
        new_node = SDL_malloc(sizeof(HashMap_Node));
        new_node->next = nullptr;
        new_node->key = key;
    }
    new_node->value = value;
    if (map->buckets[index] == nullptr) {
        map->buckets[index] = new_node;
        map->n_buckets_filled++;
        map->n_keys++;
    } else if (!key_exists) {
        last->next = new_node;
        map->n_keys++;
    }
}

void HashMap_PutInt(HashMap *map, const long key, const int value) {
    HashMap_Value v;
    v.v_int = value;
    HashMap_Put(map, key, v);
}

void HashMap_PutLong(HashMap *map, const long key, const long value) {
    HashMap_Value v;
    v.v_long = value;
    HashMap_Put(map, key, v);
}

void HashMap_PutPtr(HashMap *map, const long key, void *value) {
    HashMap_Value v;
    v.v_ptr = value;
    HashMap_Put(map, key, v);
}

int HashMap_StrGet(const HashMap *map, const char *key, HashMap_Value *out_value) {
    return HashMap_Get(map, HashMap_KeyFromString(key), out_value);
}

void HashMap_StrPut(HashMap *map, const char *key, const HashMap_Value value) {
    HashMap_Put(map, HashMap_KeyFromString(key), value);
}

void HashMap_StrPutInt(HashMap *map, const char *key, const int value) {
    HashMap_PutInt(map, HashMap_KeyFromString(key), value);
}

void HashMap_StrPutLong(HashMap *map, const char *key, const long value) {
    HashMap_PutLong(map, HashMap_KeyFromString(key), value);
}

void HashMap_StrPutPtr(HashMap *map, const char *key, void *value) {
    HashMap_PutPtr(map, HashMap_KeyFromString(key), value);
}

void HashMap_Destroy(HashMap *map) {
    // free bucket entries...
    for (size_t i = 0; i < map->cap; i++) {
        HashMap_Node *curr = map->buckets[i];
        while (curr != nullptr) {
            HashMap_Node *next = curr->next;
            SDL_free(curr);
            curr = next;
        }
    }
    SDL_free(map->buckets);
    SDL_free(map);
}
