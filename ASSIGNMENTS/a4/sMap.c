/*
 * ============================================================================
 *
 *       Filename:  s_map.c
 *
 *    Description:  Handler for thread safe hashmap data structure
 *
 *        Version:  1.0
 *        Created:  23/10/23 15:42:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4 
 *
 * ============================================================================
 */
#include <stdlib.h>
#include <stdio.h>

#include "sMap.h"

Map* map_create(
        CompareFunction* equalFunction,
        NodeFunction* destroyFunction, 
        NodeFunction* printFunction,
        pthread_mutex_t* mutex) {
    Map* map = (Map*) malloc(sizeof(Map));

    map->size = 0;
    map->capacity = INITIAL_CAPACITY;
    map->nodes = (MapNode**) calloc(sizeof(MapNode*), map->capacity);
    map->equalFunction = equalFunction;
    map->destroyFunction = destroyFunction;
    map->printFunction = printFunction;
    map->mutex = mutex;

    return map;
}

MapNode* map_create_node(void* key, void* value) {
    MapNode* node = (MapNode*) malloc(sizeof(MapNode));

    node->key = key;
    node->value = value;
    node->next = NULL;

    return node;
}

void map_insert(Map* map, void* key, void* value) {
    pthread_mutex_lock(map->mutex);

    map_insert_unsafe(map, key, value);

    pthread_mutex_unlock(map->mutex);
}

void map_insert_unsafe(Map* map, void* key, void* value) {
    if (map_get_load(map) > (float) LOAD_FACTOR_REHASH) {
        map_rehash(map);
    }

    size_t nodeIndex = map_get_index(map, key);
    MapNode* currentNode = map->nodes[nodeIndex];

    while(currentNode != NULL) {
        if (map->equalFunction(currentNode->key, key)) {
            // Node already exists in the map so update the value
            map->destroyFunction(currentNode->key, currentNode->value);

            currentNode->key = key;
            currentNode->value = value;
            return;
        }

        currentNode = currentNode->next;
    }
    
    // Node does not exist in the map
    MapNode* insertNode = map_create_node(key, value);

    // Add new node to head of linked list
    insertNode->next = map->nodes[nodeIndex];
    map->nodes[nodeIndex] = insertNode;
    
    map->size++;
}

void* map_find(Map* map, void* key) {
    pthread_mutex_lock(map->mutex);

    size_t nodeIndex = map_get_index(map, key);
    MapNode* currentNode = map->nodes[nodeIndex];
    
    while (currentNode != NULL) {
        if (map->equalFunction(currentNode->key, key)) {
            void* value = currentNode->value;

            pthread_mutex_unlock(map->mutex);
            return value;
        }

        currentNode = currentNode->next;
    }
 
    pthread_mutex_unlock(map->mutex);
    return NULL;
}

void map_remove(Map* map, void* key) {
    pthread_mutex_lock(map->mutex);

    size_t nodeIndex = map_get_index(map, key);

    MapNode* currentNode = map->nodes[nodeIndex];
    MapNode* previousNode = NULL;
    
    while (currentNode != NULL) {
        if (map->equalFunction(currentNode->key, key)) {
            // Node exists in the map so remove and free
            break;    
        }

        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    if (currentNode == NULL) {
        return;
    }

    if (previousNode == NULL) {
        map->nodes[nodeIndex] = currentNode->next;
    } else {
        previousNode->next = currentNode->next;
    }

    // Free the memory related to the node
    map->destroyFunction(currentNode->key, currentNode->value);
    free(currentNode);
    map->size--;

    pthread_mutex_unlock(map->mutex);
}

void map_rehash(Map* map) {
    size_t oldCapacity = map->capacity;
    size_t oldSize = map->size;
    MapNode** oldNodes = map->nodes; 

    map->size = 0;
    map->capacity *= GROWTH_FACTOR;
    
    map->nodes = (MapNode**) calloc(sizeof(MapNode*), map->capacity);

    size_t currentSize = 0;

    for (size_t i = 0; i < oldCapacity; i++) {
        if (currentSize == oldSize) {
            break;
        }

        MapNode* node = oldNodes[i];

        while (node != NULL) {
            MapNode* temp = node;

            map_insert_unsafe(map, node->key, node->value);

            node = temp->next;

            free(temp);

            currentSize++;
        }
    }

    free(oldNodes);
}

float map_get_load(Map* map) {
    float loadFactor = (float) map->size / (float) map->capacity;

    return loadFactor;
}

size_t map_get_index(Map* map, void* key) {
    size_t nodeIndex = map_hash_function(key, HASH_SEED) % map->capacity;
    return nodeIndex;
}

size_t map_hash_function(void* key, size_t seed) {
    char* wordKey = (char*) key;

    // Hash word key using MurmurHash
    for (; *wordKey; ++wordKey) {
        seed ^= *wordKey;
        seed *= 0x5bd1e995;
        seed ^= seed >> 15;
    }
    return seed;
}

void map_print(void* object) {
    Map* map = (Map*) object;
    
    pthread_mutex_lock(map->mutex);
    
    size_t currentSize = 0;
    
    printf("Map Size: %ld\n{", map->size);

    for (size_t i = 0; i < map->capacity; i++) {
        if (currentSize == map->size) {
            break;
        }

        MapNode* node = map->nodes[i];
        
        while (node != NULL) {
            map->printFunction(node->key, node->value);

            printf(", ");

            node = node->next;
            currentSize++;
        }
    }

    printf("}\n");
    pthread_mutex_unlock(map->mutex);
}

void map_destroy(void* object) {
    Map* map = (Map*) object;
    
    pthread_mutex_t* mutex = map->mutex;
    pthread_mutex_lock(mutex);

    for (size_t i = 0; i < map->capacity; i++) {
        while (map->nodes[i] != NULL) {
            MapNode* node = map->nodes[i];

            map->nodes[i] = map->nodes[i]->next;
            map->destroyFunction(node->key, node->value);
            free(node);
        }
    }

    free(map->nodes);
    free(map);

    pthread_mutex_unlock(mutex);
}
