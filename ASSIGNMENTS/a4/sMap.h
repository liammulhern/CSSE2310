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

/* CompareFunction
 * ---------------
 * Function pointer prototype for comparing 2 hash table keys and returning
 * true if they are considered the same, false otherwise.
 * 
 * Parameters:
 *  void* : The first key to compare. Should be type cast to intended 
 *  type before comparing.
 *  void* : The second key to compare. Should be type cast to intended 
 *  type before comparing.
 *
 * Returns:
 *  True if keys are considered the same, false otherwise.
 * 
 * Errors:
 *  None
 */
typedef bool CompareFunction(void*, void*);

/* NodeFunction 
 * ---------------
 * Performs operation on key/value pair of map.
 * 
 * Parameters:
 *  void* : The key to of the MapNode. Should be type cast to intended 
 *  type before comparing.
 *  void* : The value of the MapNode. Should be type cast to intended 
 *  type before comparing.
 *
 * Returns:
 *  True if keys are considered the same, false otherwise.
 * 
 * Errors:
 *  None
 */
typedef void NodeFunction(void*, void*);

/* MapNode 
 * ---------------
 * Struct representing key/value pair stored in Map.
 * 
 * Parameters:
 *  key: The generic key used to identity the node.
 *  value: The generic object to be stored as the value of the node.
 *  next: The next MapNode if a collision occurs in the table.
 *  previous: The previous MapNode if a collision occurs in the table.
 */
typedef struct MapNode {
    void* key;
    void* value;
    struct MapNode* next;
    struct MapNode* previous;
} MapNode;

/* Map 
 * ---------------
 * Thread safe struct representing a Map's state.
 * 
 * Parameters: 
 *  equalFunction: Function pointer used to compare keys when inserting 
 *      and finding.
 *  destroyFunction: Function pointer used when freeing a key's 
 *      allocated memory.
 *  printFunction: Function pointer used to print node to stdout.
 *  nodes: 2D array of key/value nodes stored in table.
 *  capacity: The total number of nodes that a map can contain 
 *  size: The number of locations in the table where MapNode pointer 
 *      can be stored.
 *  mutex: The mutex used for thread safe methods.
 */
typedef struct Map {
    CompareFunction* equalFunction;
    NodeFunction* destroyFunction;
    NodeFunction* printFunction;
    MapNode** nodes;
    size_t capacity;
    size_t size;
    pthread_mutex_t* mutex;
} Map;

/* map_create
 * ---------------
 * Creates map data structure of given size and returns pointer.
 * 
 * Parameters:
 *  equalFunction: Function pointer used to compare keys when inserting 
 *      and finding.
 *  destroyFunction: Function pointer used when freeing a key's 
 *      allocated memory.
 *  printFunction: Function pointer used to print node to stdout.
 *  mutex: The mutex used for thread safe methods.
 *
 * Returns:
 *  Pointer to allocated Map.
 * 
 * Errors:
 *  None
 */ 
Map* map_create(
        CompareFunction* equalFunction,
        NodeFunction* destroyFunction, 
        NodeFunction* printFunction,
        pthread_mutex_t* mutex);

/* map_create_node
 * ---------------
 * Initialise and allocate a MapNode data structure.
 * 
 * Parameters:
 *  key: The key of the MapNode.
 *  value: The value of the MapNode.
 *
 * Returns:
 *  Pointer to the allocate MapNode.
 * 
 * Errors:
 *  None
 */
MapNode* map_create_node(void* key, void* value);

/* map_insert
 * ---------------
 * Thread safe insert object into Map with given key. 
 * 
 * Parameters:
 *  map: Pointer to map where object will be inserted.
 *  key: Generic key used to link pointer with Map address.
 *  value: Generic pointer to object being added to Map.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void map_insert(Map* map, void* key, void* value);

/* map_insert_unsafe
 * ---------------
 * Insert object into Map with given key. 
 * 
 * Parameters:
 *  map: Pointer to map where object will be inserted.
 *  key: Generic key used to link pointer with Map address.
 *  value: Generic pointer to object being added to Map.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void map_insert_unsafe(Map* map, void* key, void* value);

/* map_find
 * ---------------
 * Thread safe find object from Map using compare function to 
 * compare keys with given key and return generic object pointer.
 * 
 * Parameters:
 *  Map: Pointer to Map where get function will run.
 *  key: Generic key used to link pointer with Map address.
 *
 * Returns:
 *  Generic pointer to object with key, NULL if not found. 
 * 
 * Errors:
 *  Returns NULL if key is not found in table.
 */
void* map_find(Map* map, void* key);

/* map_remove
 * ---------------
 * Thread safe remove object from Map with given key. 
 * 
 * Parameters:
 *  map: Pointer to Map where remove function will run.
 *  key: String used to define hashFunction to Map address.
 *
 * Returns:
 *  None
 *
 * Errors:
 *  None 
 */
void map_remove(Map* map, void* key);

/* map_rehash
 * ---------------
 * Thread safe rehash the map and scale capacity by GROWTH_FACTOR.
 * 
 * Parameters:
 *  map: Pointer to Map where remove function will run.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void map_rehash(Map* map);

/* map_get_load
 * ---------------
 * Get the load factor of the map.
 * 
 * Parameters:
 *  map: Pointer to Map where remove function will run.
 *
 * Returns:
 *  The decimal load factor of the map.
 * 
 * Errors:
 *  None
 */
float map_get_load(Map* map);

/* map_get_index 
 * ---------------
 * Generate the map index from a key and hashFunction function.
 * 
 * Parameters:
 *  map: Pointer to allocated Map.
 *  key: String used to define hashFunction to HashTable address.
 *
 * Returns:
 *  Hash index of key.
 * 
 * Errors:
 *  None
 */
size_t map_get_index(Map* map, void* key);

/* map_hash_function
 * ---------------
 * Hashing function based on MurmurHash or Murmur One Byte At a Time 
 * hashFunction method. Implements HashFunction function pointer typedef.
 * 
 * Parameters:
 *  key: String used to define hashFunction to HashTable address.
 *  seed: Seed value for hashFunction generation.
 *
 * Returns:
 *  Returns hash for given key.
 * 
 * Errors:
 *  None
 *
 * REF: Hash function is inspired by Austin Appleby's MurmurHash hosted at
 * REF: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
 */
size_t map_hash_function(void* key, size_t seed);

/* map_print
 * ---------------
 * Thread safe print Map contents to stdout. 
 * Implements PrintFunction function pointer typedef.
 * 
 * Parameters:
 *  object: Generic pointer to Map that will be printed.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void map_print(void* object);

/* map_destroy
 * ---------------
 * Thread safe destroy Map and free memory.
 * Implements DestroyFunction function pointer typdef.
 *
 * Parameters:
 *  object: generic pointer that will be cast to Map to
 *  free allocated memory associated with the Mao.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void map_destroy(void* object);

#endif
