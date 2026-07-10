/*
 * ============================================================================
 *
 *       fileName:  hashtable.h
 *
 *    Description:  Header file for hashtable.c. 
 *
 *        Version:  1.0
 *        Created:  06/08/23 19:42:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern 
 *   Organization:  CSSE2310 A1 
 *
 * ============================================================================
 */
#ifndef HASHTABLE_H
#define HASHTABLE_H

#define DEFAULT_SEED 0x12345678

#include <stddef.h>
#include <stdbool.h>

#include "destroy.h"
#include "print.h"

/* HashFunction()
 * ---------------
 * Function pointer prototype for generic hashing function that returns
 * a hashed number that is the largest type the system can support.
 * 
 * Parameters:
 *  void* : Generic object pointer that represents the key to hash. 
 *  Should be type cast to intended type before hashing.
 *  size_t : Represents the size (in bytes) of the key that will be hashed.
 *  size_t : Represents the seed used to hash the key.
 *
 * Returns:
 *  Hashed number that is the largest type the system can support.
 * 
 * Errors:
 *  None
 */
typedef size_t HashFunction (void*, size_t, size_t);

/* HashCompareFunction()
 * ---------------
 * Function pointer prototype for comparing 2 hash table keys and returning
 * true if they are considered the same, false otherwise.
 * 
 * Parameters:
 *  void* : The first key to compare. Should be type cast to intended 
 *  type before comparing.
 *  void* : The second key to compare. Should be type cast to intended 
 *  type before comparing.
 *  size_t: The size (in bytes) of the keys.
 *
 * Returns:
 *  True if keys are considered the same, false otherwise.
 * 
 * Errors:
 *  None
 */
typedef bool HashCompareFunction (void*,  void*, size_t);

/* HashTableNode
 * ---------------
 * Struct representing key/value pair stored in HashTable.
 * 
 * Parameters:
 *  key: The generic key used to identity the node.
 *  keySize: The size of the key (in bytes).
 *  object: The generic object to be stored as the value of the node.
 *  next: The next HashTableNode if a collision occurs in the table.
 */
typedef struct HashTableNode {
    void* key;
    size_t keySize;
    void* object;
    struct HashTableNode* next;
} HashTableNode;

/* HashTable
 * ---------------
 * Struct representing a HashTable's state.
 * 
 * Parameters:
 *  size: The number of locations in the table where HashTableNode pointer 
 *  can be stored.
 *  count: The number of HashTableNodes in the HashTable.
 *  hashFunction: Function pointer used to hash a table's key.
 *  compareKeyFunction: Function pointer used to compare keys when inserting 
 *  and finding.
 *  destroyKeyFunction: Function pointer used when freeing a key's 
 *  allocated memory.
 *  printKeyFunction: Function pointer used to print key to stdout.
 *  destroyNodeFunction: Function pointer used when freeing a value object's
 *  allocated memory.
 *  printNodeFunction: Function pointer used to print node to stdout.
 *  nodes: 2D array of key/value nodes stored in table.
 */
typedef struct HashTable {
    size_t size;
    size_t count;
    HashFunction* hashFunction;
    HashCompareFunction* compareKeyFunction;
    DestroyFunction* destroyKeyFunction;
    PrintFunction* printKeyFunction;
    DestroyFunction* destroyNodeFunction;
    PrintFunction* printNodeFunction;
    HashTableNode** nodes;
} HashTable;

/* hash_table_create()
 * ---------------
 * Creates HashTable data structure of given size and returns pointer.
 * 
 * Parameters:
 *  size: The number of locations in the table where HashTableNode pointer 
 *  can be stored.
 *  hashFunction: Function pointer used to hash a table's key.
 *  compareKeyFunction: Function pointer used to compare keys when inserting 
 *  and finding.
 *  destroyKeyFunction: Function pointer used when freeing a key's 
 *  allocated memory.
 *  printKeyFunction: Function pointer used to print key to stdout.
 *  destroyNodeFunction: Function pointer used when freeing a value object's
 *  allocated memory.
 *  printNodeFunction: Function pointer used to print node to stdout.
 *
 * Returns:
 *  Pointer to allocated HashTable of given size.
 * 
 * Errors:
 *  None
 */
HashTable* hash_table_create(
        size_t size, 
        HashFunction* hashFunction, 
        HashCompareFunction compareFunction, 
        DestroyFunction* destroyKeyFunction,
        PrintFunction* printKeyFunction,
        DestroyFunction* destroyFunction,
        PrintFunction printFunction);

/* hash_table_destroy()
 * ---------------
 * destroyNodeFunction HashTable and free memory.
 * Implements DestroyFunction function pointer typdef.
 *
 * Parameters:
 *  object: generic pointer that will be cast to HashTable to
 *  free allocated memory associated with the tablee.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void hash_table_destroy(void* object);

/* hash_table_index()
 * ---------------
 * Generate the table index from a key and hashFunction function.
 * 
 * Parameters:
 *  hashTable: Pointer to allocated HashTable.
 *  key: String used to define hashFunction to HashTable address.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
size_t hash_table_index(HashTable* hashTable, void* key, size_t keySize);

/* hash_table_insert()
 * ---------------
 * Insert object into HashTable with given key. If added successfully 
 * returns true, otherwise false.
 * 
 * Parameters:
 *  hashTable: Pointer to HashTable where object will be inserted.
 *  key: Generic key used to link pointer with HashTable address.
 *  keySize: The size of the key in bytes.
 *  object: Generic pointer to object being added to HashTable.
 *
 * Returns:
 *  If object is added to HashTable returns true, otherwise false.
 * 
 * Errors:
 *  None
 */
bool hash_table_insert(
        HashTable* hashTable, 
        void* key, 
        size_t keySize,
        void* object);

/* hash_table_get()
 * ---------------
 * Get object from HashTable using default compare function to 
 * compare keys with given key and return generic object pointer.
 * 
 * Parameters:
 *  hashTable: Pointer to HashTable where get function will run.
 *  key: Generic key used to link pointer with HashTable address.
 *  keySize: The size of the key in bytes.
 *
 * Returns:
 *  Generic pointer to object with key, NULL if not found. 
 * 
 * Errors:
 *  Returns NULL if key is not found in table.
 */
void* hash_table_get(
        HashTable* hashTable, 
        void* key, 
        size_t keySize);

/* hash_table_get_compare()
 * ---------------
 * Get object from HashTable using given compare function to
 * compare keys with given key and return generic object pointer.
 * 
 * Parameters:
 *  hashTable: Pointer to HashTable where get function will run.
 *  key: String used to define hashFunction to HashTable address.
 *  keySize: The byte length of the key.
 *  compare: The function to compare the keys.
 *
 * Returns:
 *  Pointer to returned object, NULL if not found. 
 * 
 * Errors:
 *  Returns NULL if key is not found in table.
 */
void* hash_table_get_compare(
        HashTable* hashTable, 
        void* key, 
        size_t keySize, 
        HashCompareFunction compare);

/* hash_table_key_exists()
 * ---------------
 * Returns true if the key exists in the hashtable using default 
 * compare function, false otherwise.
 * 
 * Parameters:
 *  hashTable: Pointer to HashTable where exists function will run.
 *  key: String used to define hashFunction to HashTable address.
 *  keySize: The byte length of the key.
 * 
 * Returns:
 *  True if the key exists in the hashtable, false otherwise.
 * 
 * Errors:
 *  None
 */
bool hash_table_key_exists(
        HashTable* hashTable, 
        void* key, 
        size_t keySize);

/* hash_table_key_exists_compare()
 * ---------------
 * Returns true if the key exists in the hashtable when using the 
 * compare function, false otherwise.
 * 
 * Parameters:
 *  hashTable: Pointer to HashTable where exists function will run.
 *  key: String used to define hashFunction to HashTable address.
 *  keySize: The byte length of the key.
 *  compare: The function used to compare keys.
 *
 * Returns:
 *  True if the key exists in the hashtable when using the 
 *  compare function, false otherwise.
 * 
 * Errors:
 *  None
 */
bool hash_table_key_exists_compare(HashTable* hashTable, 
        void* key, 
        size_t keySize,
        HashCompareFunction compare);

/* hash_table_remove()
 * ---------------
 * Remove object from HashTable with given key. 
 * 
 * Parameters:
 *  hashTable: Pointer to HashTable where remove function will run.
 *  key: String used to define hashFunction to HashTable address.
 *  keySize: The byte length of the key.
 *
 * Returns:
 *  None
 *
 * Errors:
 *  None 
 */
void hash_table_remove(
        HashTable* hashTable, 
        void* key, 
        size_t keySize);

/* hash_table_iterate()
 * ---------------
 * Iterate over a HashTable and return the node and its index in the 
 * currentNode and index parameters. Iteration returns NULL when the HashTable
 * contains no more nodes.
 * 
 * Parameters:
 *  hashTable: Pointer to HashTable to iterate over.
 *  currentNode: The pointer to the returned node (Initialise to NULL).
 *  index: The HashTable index of the node (Initialise to 0);
 *
 * Returns:
 *  The node at the index, or NULL when the iteration finishes.
 * 
 * Errors:
 *  None
 */
HashTableNode* hash_table_iterate(
        HashTable* hashTable,
        HashTableNode* currentNode, 
        size_t* index);

/* hash_table_empty()
 * ---------------
 * Returns true if the hash table contains no HashTableNodes. Otherwise false.
 * 
 * Parameters:
 *  hashTable: The HashTable to check.
 *
 * Returns:
 *  Returns true if the hash table contains no HashTableNodes. 
 *      Otherwise false.
 * 
 * Errors:
 *  None
 */
bool hash_table_empty(HashTable* hashTable);

/* hash_table_print()
 * ---------------
 * Print HashTable contents to stdout. Implements PrintFunction function 
 * pointer typedef.
 * 
 * Parameters:
 *  object: Generic pointer to HashTable that will be printed.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void hash_table_print(void* object);

/* hash_table_print_node()
 * ---------------
 * Prints a HashTableNode's key and value object using the print functions
 * given at initialisation.
 * 
 * Parameters:
 *  hashTable: The HashTable the node belongs to.
 *  node: The HashTableNode that will be printed.
 *  index: The index of the HashTableNode.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void hash_table_print_node(
        HashTable* hashTable, 
        HashTableNode* node, 
        size_t index);

/* hash_function()
 * ---------------
 * Hashing function based on MurmurHash or Murmur One Byte At a Time 
 * hashFunction method. Implements HashFunction function pointer typedef.
 * 
 * Parameters:
 *  key: String used to define hashFunction to HashTable address.
 *  length: The length of the key.
 *  seed: Seed value for hashFunction generation.
 *
 * Returns:
 *  Returns hashFunction for given key.
 * 
 * Errors:
 *  None
 *
 * REF: Hash function is inspired by Austin Appleby's MurmurHash hosted at
 * REF: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
 */
size_t hash_function(void* key, size_t keySize, size_t seed);

/* hash_table_compare_char()
 * ---------------
 * Compares 2 char pointers, cast from generic pointers, and if they are the
 * same word returns ture, false otherwise. Implements HashCompareFunction
 * function pointer typedef.
 * 
 * Parameters:
 *  key1: First pointer to compare.
 *  key2: Second pointer to compare.
 *  keySize: The size (in Bytes) of the key.
 *
 * Returns:
 *  True if the pointer words are the same, false otherwise.
 * 
 * Errors:
 *  None
 */
bool hash_table_compare_char(
        void* key1, 
        void* key2, 
        size_t keySize);

/* hash_table_compare_pointer()
 * ---------------
 * Compares 2 generic key pointers, if they are the same returns true, 
 * false otherwise. Implements HashCompareFunction
 * function pointer typedef.
 * 
 * Parameters:
 *  key1: First pointer to compare.
 *  key2: Second pointer to compare.
 *  keySize: The size (in Bytes) of the key.
 *
 * Returns:
 *  True if the pointers are the same, false otherwise.
 * 
 * Errors:
 *  None
 */
bool hash_table_compare_pointer(
        void* key1, 
        void* key2, 
        size_t keySize);

/* hash_table_compare_long()
 * ---------------
 * Compares 2 long pointers, cast from generic pointers, and if they are the
 * same returns ture, false otherwise. Implements HashCompareFunction
 * function pointer typedef.
 * 
 * Parameters:
 *  key1: First pointer to compare.
 *  key2: Second pointer to compare.
 *  keySize: The size (in Bytes) of the key.
 *
 * Returns:
 *  True if the pointer words are the same, false otherwise.
 * 
 * Errors:
 *  None
 */
bool hash_table_compare_long(
        void* key1, 
        void* key2, 
        size_t keySize);

#endif
