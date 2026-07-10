/*
 * ============================================================================
 *
 *       Filename:  s_map.h
 *
 *    Description:  Header file for s_map.c
 *
 *        Version:  1.0
 *        Created:  23/10/23 15:43:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4 
 *
 * ============================================================================
 */
#ifndef S_MAP_H
#define S_MAP_H

#define INITIAL_CAPACITY 8
#define LOAD_FACTOR_REHASH 0.66
#define HASH_SEED 146816784
#define GROWTH_FACTOR 3

#include <pthread.h>
#include <stdbool.h>

typedef bool CompareFunction(void*, void*);
typedef void NodeFunction(void*, void*);

typedef struct MapNode {
    void* key;
    void* value;
    struct MapNode* next;
    struct MapNode* previous;
} MapNode;

typedef struct Map {
    CompareFunction* equalFunction;
    NodeFunction* destroyFunction;
    NodeFunction* printFunction;
    MapNode** nodes;
    size_t capacity;
    size_t size;
    pthread_mutex_t* mutex;
} Map;

Map* map_create(
        CompareFunction* equalFunction,
        NodeFunction* destroyFunction, 
        NodeFunction* printFunction,
        pthread_mutex_t* mutex);

MapNode* map_create_node(void* key, void* value);

void map_insert(Map* map, void* key, void* value);

void map_insert_unsafe(Map* map, void* key, void* value);

void* map_find(Map* map, void* key);

void map_remove(Map* map, void* key);

void map_rehash(Map* map);

float map_get_load(Map* map);

size_t map_get_index(Map* map, void* key);

size_t map_hash_function(void* key, size_t seed);

void map_print(void* object);

void map_destroy(void* object);

#endif
