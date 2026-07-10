/*
 * ============================================================================
 *
 *       fileName:  hashtable.c
 *
 *    Description:  Provides functionality for seperate chaining hashtable 
 *          data structure and associated CRUD functions. 
 *
 *        Version:  1.0
 *        Created:  06/08/23 19:39:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern
 *   Organization:  CSSE2310 A1 
 *
 * ============================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashtable.h"

HashTable* hash_table_create(
        size_t size, 
        HashFunction* hashFunction,
        HashCompareFunction compareKeyFunction, 
        DestroyFunction destroyKeyFunction, 
        PrintFunction* printKeyFunction,
        DestroyFunction* destroyNodeFunction, 
        PrintFunction printNodeFunction) {

    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
    hashTable->size = size;
    hashTable->count = 0;
    hashTable->hashFunction = hashFunction;

    hashTable->compareKeyFunction = compareKeyFunction;
    hashTable->destroyKeyFunction = destroyKeyFunction;
    hashTable->printKeyFunction = printKeyFunction;

    hashTable->destroyNodeFunction = destroyNodeFunction;
    hashTable->printNodeFunction = printNodeFunction; 

    // Initialises object data to 0
    hashTable->nodes = (HashTableNode**)calloc(sizeof(HashTableNode), 
            hashTable->size);

    return hashTable;
}

void hash_table_destroy(void* object) {
    HashTable* hashTable = (HashTable*) object;
    
    // Free memory used for storing object data in nodes arrat in hashTable
    for (unsigned int i = 0; i < hashTable->size; i++) {
        // Iterate over each object in hashFunction index
        while (hashTable->nodes[i]) {
            HashTableNode* temp = hashTable->nodes[i];

            // Collapse linked list 
            hashTable->nodes[i] = hashTable->nodes[i]->next;

            // Free memory
            hashTable->destroyKeyFunction(temp->key);
            hashTable->destroyNodeFunction(temp->object);
            free(temp);
        }
    }

    free(hashTable->nodes);
    free(hashTable);
}

size_t hash_table_index(
        HashTable* hashTable, 
        void* key,
        size_t keySize) {
    // Generate hashed index within HashTable bounds (i.e 0 -> size).
    size_t hashIndex = (hashTable->hashFunction(key, keySize, DEFAULT_SEED) 
            % hashTable->size);

    return hashIndex;
}

bool hash_table_insert(
        HashTable* hashTable, 
        void* key, 
        size_t keySize, 
        void* object) {
    if (key == NULL || object == NULL || hashTable == NULL) {
        return false;
    }

    if (hash_table_get(hashTable, key, keySize) != NULL) {
        return false;
    }

    size_t hashIndex = hash_table_index(hashTable, key, keySize);

    // Create new object that will be inserted into the HashTable
    HashTableNode* node = (HashTableNode*)malloc(sizeof(HashTableNode));
    node->object = object;

    node->key = key;
    node->keySize = keySize;

    // Update the chaining linked list 
    node->next = hashTable->nodes[hashIndex];
    hashTable->nodes[hashIndex] = node;

    hashTable->count++;

    return true;
}

void* hash_table_get(
        HashTable* hashTable, 
        void* key, 
        size_t keySize) {

    void* object = hash_table_get_compare(hashTable, key,
            keySize, hashTable->compareKeyFunction);

    return object;
}

void* hash_table_get_compare(
        HashTable* hashTable, 
        void* key, 
        size_t keySize, 
        HashCompareFunction compare) {

    if (key == NULL || hashTable == NULL) {
        return NULL;
    }

    size_t hashIndex = hash_table_index(hashTable, key, keySize);
    HashTableNode* node = hashTable->nodes[hashIndex];

    // Iterate over linked list until the end is reached or the key is found
    while (node != NULL && !(compare(node->key, key, keySize))) {
        node = node->next;
    }

    // Could not find object with key
    if (node == NULL) {
        return NULL;
    }

    return node->object;
}

bool hash_table_key_exists(
        HashTable* hashTable, 
        void* key, 
        size_t keySize) {

    if (key == NULL || keySize == 0 || hashTable == NULL) {
        return false;
    }

    return hash_table_get(hashTable, key, keySize) != NULL;
}

bool hash_table_key_exists_compare(
        HashTable* hashTable, 
        void* key, 
        size_t keySize,
        HashCompareFunction compare) {

    if (key == NULL || keySize == 0 || hashTable == NULL) {
        return false;
    }

    return hash_table_get_compare(hashTable, key, keySize, compare) != NULL;
}

void hash_table_remove(HashTable* hashTable, void* key, size_t keySize) {
    if (key == NULL || keySize == 0 || hashTable == NULL) {
        return;
    }

    size_t hashIndex = hash_table_index(hashTable, key, keySize);

    HashTableNode* currentNode = hashTable->nodes[hashIndex];
    HashTableNode* previousNode = NULL;

    // Iterate over linked list until the end is reached or the key is found
    while (currentNode != NULL && !(hashTable->compareKeyFunction(
                    currentNode->key, key, keySize))) {
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    // Could not find object with key
    if (currentNode == NULL) {
        return;
    }

    // Check if removing the head of the linked list,
    if (previousNode == NULL) {
        hashTable->nodes[hashIndex] = currentNode->next;
    } else {
        previousNode->next = currentNode->next;
    }

    hashTable->count--;
    free(currentNode);
}

HashTableNode* hash_table_iterate(
        HashTable* hashTable, 
        HashTableNode* currentNode, 
        size_t* index) {

    if (*index >= hashTable->size) {
        return NULL;
    } 

    if (currentNode != NULL) {
        // If current node is part of a linked list, iterate over children
        // nodes before incrementing index.
        if (currentNode->next != NULL) {
            currentNode = currentNode->next;

            return currentNode;
        } else {
            (*index)++;
        }
    }

    // Increment the index of the hashtable until the next node is found.
    while (hashTable->nodes[*index] == NULL) {
        if (*index >= hashTable->size) {
            return NULL;
        } 

        (*index)++;
    }

    currentNode = hashTable->nodes[*index];

    return currentNode; 
}

bool hash_table_empty(HashTable* hashTable) {
    return hashTable->count == 0;
}

void hash_table_print(void* object) {
    if (object == NULL) {
        return;
    }

    HashTable* hashTable = (HashTable*) object;

    printf("\nTable Size: %ld\n", hashTable->size);
    printf("-----------\n");
    
    size_t index = 0;
    HashTableNode* currentNode = NULL; 

    // Iterate of hashTable nodes until end and print to stdout.
    while ((currentNode = hash_table_iterate(
                    hashTable, currentNode, &index)) != NULL) {

        hash_table_print_node(hashTable, currentNode, index);
        printf("\n");
    }

    printf("----END----\n");
}

void hash_table_print_node(
        HashTable* hashTable, 
        HashTableNode* node, 
        size_t index) {

    printf("%ld : ", index);
    printf("[");
    hashTable->printKeyFunction(node->key);
    printf("] -> ");
    hashTable->printNodeFunction(node->object);
}

size_t hash_function(void* key, size_t keySize, size_t seed) {
    char* wordKey = (char*) key;

    // Hash word key using MurmurHash
    for (; *wordKey; ++wordKey) {
        seed ^= *wordKey;
        seed *= 0x5bd1e995;
        seed ^= seed >> 15;
    }
    return seed;
}

bool hash_table_compare_char(
        void* key1, 
        void* key2, 
        size_t keySize) {
    char* wordKey1 = (char*) key1;
    char* wordKey2 = (char*) key2;

    return strcmp(wordKey1, wordKey2) == 0;
}

bool hash_table_compare_pointer(
        void* key1, 
        void* key2, 
        size_t keySize) {
    return key1 == key2;
}

bool hash_table_compare_long(
        void* key1, 
        void* key2, 
        size_t keySize) {
    long number1 = *((long*) key1);
    long number2 = *((long*) key2);

    return number1 == number2;
}
