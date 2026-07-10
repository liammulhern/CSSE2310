/*
 * ============================================================================
 *
 *       fileName:  dictionary.c
 *
 *    Description:  Reads and dynamically allocates dictionary words to
 *		    memory. 
 *
 *        Version:  1.0
 *        Created:  06/08/23 18:25:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern
 *   Organization:  CSSE2310 A1 
 *
 * ============================================================================
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"
#include "list.h"
#include "print.h"
#include "validity.h"

HashTable* dictionary_get_words(
	char* fileName, 
	unsigned short inputWordLength) {

    FILE* wordList = fopen(fileName, "r");
    
    if (validity_check_dictionary_filename(fileName, wordList)) {
	return NULL;
    }
    
    char wordBuffer[WORD_MAX_SIZE];
    long currentLine = 0;
    
    // Create HashTable that will be used to store char* words as keys 
    // and long ints as values
    HashTable* wordDictionary = hash_table_create(
	    WORD_DICTIONARY_SIZE, hash_function, 
	    hash_table_compare_pointer, free,
	    print_string_pointer, free, print_long_pointer);
 
    // Iterate over dictionary file until EOF 
    while (fgets(wordBuffer, WORD_MAX_SIZE, wordList) != NULL) {
	// Get length of string to the new line character
	size_t wordLength = strcspn(wordBuffer, "\n");

	// Check if word is valid, skip to next iteration if not.
	if (!dictionary_check_word_validity(wordBuffer, wordLength, 
		inputWordLength)) {
	    continue;
	}
	
	// Shrink wordBuffer and allocate memory for a word's index
	char* word = dictionary_shrink_word(wordBuffer, wordLength);
	long* index = (long*) malloc(sizeof(long));
	*index = currentLine++;

	hash_table_insert(wordDictionary, word, wordLength, index);
    }

    fclose(wordList);
    
    return wordDictionary; 
}

HashTable* dictionary_create_word_buckets(HashTable* wordDictionary) {

    // Create dictionary of word buckets that are contain lists of 
    // words 1 character different.
    HashTable* bucketDictionary = hash_table_create(BUCKET_DICTIONARY_SIZE,
	    hash_function, hash_table_compare_char, free, 
	    print_string_pointer, list_destroy, list_print);
     
    size_t index = 0;
    HashTableNode* currentNode = NULL; 

    // Iterate over hash table and create word buckets
    while ((currentNode = hash_table_iterate(wordDictionary, 
		currentNode, &index)) != NULL) {
	dictionary_create_word_bucket(bucketDictionary, currentNode);
    }
    
    return bucketDictionary;
}

void dictionary_create_word_bucket(
	HashTable* bucketDictionary, 
	HashTableNode* wordDictionaryNode) {
    
    char* word = (char*) wordDictionaryNode->key;
    int wordLength = strlen(word);

    for (int j = 0; j < wordLength; j++) {
	// Create local copy of word so that wildcard can be placed at each 
	// index
	char* wordKey = strdup(word);
	wordKey[j] = WORD_WILDCARD_CHAR; 

	if (!hash_table_key_exists(bucketDictionary, wordKey, 
		wordLength)) {
	    // If the bucket doesn't exist create linked List
	    // and append word that matches pattern before inserting
	    // into HashTable
	    List* wordBucket = list_create(destroy_null, 
		    print_string_pointer);

	    list_append(wordBucket, word);
	    hash_table_insert(bucketDictionary, wordKey, wordLength,
		    wordBucket);
	} else {
	    // If the bucket exists append to bucket linked list
	    List* wordBucket = (List*) hash_table_get(bucketDictionary, 
		    wordKey, wordLength);

	    list_append(wordBucket, word);
	    free(wordKey);
	}
    }
}

HashTable* dictionary_create_word_graph(
	HashTable* bucketDictionary, 
	HashTable* wordDictionary) {

    // Create dictionary graph that contains HashTable values to store
    // suggestion words
    HashTable* suggestionDictionary = hash_table_create(
	    SUGGESTION_DICTIONARY_SIZE, hash_function, 
	    hash_table_compare_char, destroy_null, 
	    print_string, hash_table_destroy, hash_table_print);
    
    // Iterate over bucket dictionary and create connecting edges for words.
    size_t index = 0;
    HashTableNode* currentNode = NULL; 

    while ((currentNode = hash_table_iterate(
		bucketDictionary, currentNode, &index)) != NULL) {
	dictionary_create_word_graph_edges(suggestionDictionary, 
		wordDictionary, currentNode);
    }

    return suggestionDictionary;
}

void dictionary_create_word_graph_edges(
	HashTable* suggestionDictionary, 
	HashTable* wordDictionary, 
	HashTableNode* bucketDictionaryNode) {
    
    List* words = (List*) bucketDictionaryNode->object;
    size_t wordsSize= words->size;
   
    // Get the cartesian product of the bucket's word list
    for (int i = 0; i < wordsSize; i++) {
	char* word1 = (char*) list_get(words, i);
	
	for (int j = 0; j < wordsSize; j++) {
	    char* word2 = (char*) list_get(words, j);

	    // If words are different add each word to the others similar
	    // word HashTable
	    if (strcmp(word1, word2) != 0) {
		dictionary_create_word_graph_edge(suggestionDictionary, 
			wordDictionary, word1, word2);
		dictionary_create_word_graph_edge(suggestionDictionary, 
			wordDictionary, word2, word1);
	    }
	}
    }
}

void dictionary_create_word_graph_edge(
	HashTable* suggestionDictionary, 
	HashTable* wordDictionary, 
	char* wordKey, 
	char* edgeWord) {
	
    size_t edgeWordLength = strlen(edgeWord);
    size_t wordLength = strlen(wordKey);

    long* index = (long*) hash_table_get(wordDictionary, edgeWord, 
	    edgeWordLength);

    if (!hash_table_key_exists(suggestionDictionary, wordKey, wordLength)) {
	// Create edgeWord HashTable to store similar words 
	// if key word is not suggestionDictionary
	HashTable* edges = hash_table_create(EDGE_DICTIONARY_SIZE, 
		hash_function, hash_table_compare_long, destroy_null,
		print_long_pointer, destroy_null, print_string_pointer);
	
	hash_table_insert(edges, index, sizeof(long*), edgeWord);
	hash_table_insert(suggestionDictionary, wordKey, wordLength, edges);
    } else {
	// Insert edgeWord word into edgeWord HashTable if key exists
	HashTable* edges = (HashTable*) hash_table_get(suggestionDictionary, 
		wordKey, wordLength);
	
	hash_table_insert(edges, index, sizeof(long*), edgeWord);
    }
}

List* dictionary_get_suggestions(GameState* gameState) {
    HashTable* wordDictionary = gameState->wordDictionary;
    HashTable* suggestionDictionary = gameState->suggestionDictionary;
    HashTable* bucketDictionary = gameState->bucketDictionary;
    List* previousWords = gameState->previousWords;
    char* searchWord = gameState->currentWord;
    char* finalWord = gameState->arguments->finalWord;
    int freeSuggestionsFlag = 0; 
    int finalWordFlag = 0;

    HashTable* suggestionsTable = dictionary_get_suggestions_table(
	    wordDictionary, suggestionDictionary, bucketDictionary, 
	    searchWord, &freeSuggestionsFlag);
	    
    // If the suggestionTable returns NULL and the searchWord is in the 
    // dictionary, then the searchWord has no suggestions
    if (suggestionsTable == NULL) {
	return NULL;
    }
    
    // Create list of searchWord indices that will be sorted 
    List* suggestionsIndex = dictionary_create_suggestions_index_list(
	    suggestionsTable, previousWords, finalWord, &finalWordFlag);

    // Convert linked list of indices to array of longs 
    size_t suggestionsIndexSize = suggestionsIndex->size;
    long* suggestionsIndexArray = dictionary_create_suggestions_array(
	    suggestionsIndex);

    // Sort the longs based on the index to order them smallest to largest
    dictionary_sort_suggestions_array(suggestionsIndexArray, 
	    suggestionsIndexSize);

    // Create list of sorted suggestion words
    List* suggestions = dictionary_create_suggestions_list(
	    suggestionsTable, suggestionsIndexArray, 
	    suggestionsIndexSize, finalWord, finalWordFlag);

    list_destroy(suggestionsIndex);
    free(suggestionsIndexArray);
    
    // Free memory if the searchWord was unknown 
    if (freeSuggestionsFlag) {
	hash_table_destroy(suggestionsTable);
    }

    return suggestions;
}

HashTable* dictionary_get_suggestions_table(
	HashTable* wordDictionary, 
	HashTable* suggestionDictionary, 
	HashTable* bucketDictionary,
	char* word,
	int* freeFlag) {
    
    HashTable* suggestionsTable = NULL; 

    // Test if word is in dictionary (Required for initial word suggestions)
    if (!hash_table_key_exists_compare(wordDictionary, word, strlen(word),
	    hash_table_compare_char)) {
	// Get HashTable of suggested word for unknown word. 
	// Memory must be freed after suggestions list is made.
	*freeFlag = 1; 
	suggestionsTable = dictionary_get_suggestions_table_unknown(
		wordDictionary, suggestionDictionary, bucketDictionary, word);
    } else {
	// Get HashTable of suggested words for the current word
	*freeFlag = 0; 
	suggestionsTable = (HashTable*) hash_table_get(
		suggestionDictionary, word, strlen(word));
    }

    return suggestionsTable;
}

HashTable* dictionary_get_suggestions_table_unknown(
	HashTable* wordDictionary, 
	HashTable* suggestionDictionary, 
	HashTable* bucketDictionary,
	char* word) {
    
    // Create temporary suggestions table for unknown word
    HashTable* suggestionsTable = hash_table_create(EDGE_DICTIONARY_SIZE, 
	    hash_function, hash_table_compare_long, destroy_null,
	    print_long_pointer, destroy_null, print_string_pointer);
	
    int wordLength = strlen(word);

    // Create wild card word from unknown word and search word buckets for it
    for (int i = 0; i < wordLength; i++) {
	char* wordKey = strdup(word);
	wordKey[i] = WORD_WILDCARD_CHAR;

	// Check if wild card word is in buckets
	if (hash_table_key_exists(bucketDictionary, wordKey, wordLength)) {
	    // Get list of similar words that match bucket pattern.
	    List* wordBucket = (List*) hash_table_get(bucketDictionary, 
		    wordKey, wordLength);
	    // Add all similar words in bucket to the suggestions table.
	    dictionary_create_suggestions_table_unknown(wordDictionary,
		    suggestionsTable, wordBucket);
	}

	free(wordKey);
    }

    return suggestionsTable;
}

void dictionary_create_suggestions_table_unknown(
	HashTable* wordDictionary, 
	HashTable* suggestionsTable, 
	List* wordBucket) {
    size_t index = 0;
    ListNode* currentNode = NULL;

    // Iterate over bucket nodes and get suggestion word and index.
    while ((currentNode = list_iterate(wordBucket, currentNode, 
	    &index)) != NULL) {

	char* suggestion = (char*) currentNode->object;
	long* suggestionIndex = (long*) hash_table_get(wordDictionary,
		suggestion, strlen(suggestion));

	// Add suggested word to the suggestionsTable
	hash_table_insert(suggestionsTable, suggestionIndex, 
		sizeof(long*), suggestion);
    } 
}

long* dictionary_create_suggestions_array(List* list) {
    size_t size = list->size;

    long* array = malloc(sizeof(long) * size);

    // Iterate over list nodes and convert objects to long* before 
    // dereferencing and making array entry.
    size_t index = 0;
    ListNode* currentNode = NULL;

    while ((currentNode = list_iterate(list, currentNode, &index)) != NULL) {
	array[index] = *(long*) currentNode->object;
    }

    return array;
}

void dictionary_sort_suggestions_array(
	long* suggestionsIndex, 
	size_t size) {
    qsort(suggestionsIndex, size, sizeof(long), 
	    dictionary_compare_suggestions_index);
}

List* dictionary_create_suggestions_index_list(
	HashTable* suggestionsTable,
	List* previousWords,
	char* finalWord, 
	int* finalWordFlag) {
    List* suggestionsIndex = list_create(destroy_null, print_long_pointer);

    size_t index = 0;
    HashTableNode* currentNode = NULL; 

    // Iterate over all key value pairs in the suggestionTable and append The
    // index to the linked list
    while ((currentNode = hash_table_iterate(
		suggestionsTable, currentNode, &index)) != NULL) {
	char* suggestion = currentNode->object;

	// Check if suggestion list contains the target word 
	if (strcmp(suggestion, finalWord) == 0) {
	    *finalWordFlag = 1;
	} else if (list_find(previousWords, suggestion, 
		list_compare_char) == NOT_IN_LIST) {
	    // Add word indices that are not the final word index and have
	    // not been previously entered.
	    long* listIndex = (long*) currentNode->key;
	    list_append(suggestionsIndex, listIndex);
	}
    }

    return suggestionsIndex;
}

List* dictionary_create_suggestions_list(
	HashTable* suggestionsTable, 
	long* suggestionsIndexArray,
	size_t suggestionsIndexSize,
	char* finalWord,
	int finalWordFlag) {

    List* suggestions = list_create(destroy_null, print_string);
    
    // If the finalWordFlag is set add the final word first with only 
    // one occurence.
    if (finalWordFlag) {
	list_append(suggestions, finalWord);
    }

    // Iterate over the sorted index key and get the value word to append to 
    // the linked list
    for (int i = 0; i < suggestionsIndexSize; i++) {
	// Get suggestedWord using long* as key
	char* suggestedWord = (char*) hash_table_get(suggestionsTable, 
		&(suggestionsIndexArray[i]), sizeof(long*));

	list_append(suggestions, suggestedWord);
    }

    return suggestions;
}

int dictionary_compare_suggestions_index(
	const void* index1, 
	const void* index2) {

    long arg1 = *(const long*) index1;
    long arg2 = *(const long*) index2;

    return (arg1 > arg2) - (arg1 < arg2);
}

int dictionary_check_word_validity(
	char wordBuffer[WORD_MAX_SIZE], 
	size_t wordLength, 
	unsigned short inputWordLength) {

    if (wordLength != inputWordLength) {
	return 0;	
    }

    // Check if any characters are not letters and exit if any are found
    for (int i = 0; i < wordLength; i++) {
	if (!isalpha(wordBuffer[i])) {
	    return 0;
	} else {
	    // Capitalise argument character
	    wordBuffer[i] = toupper(wordBuffer[i]);
	}
    }

    return 1;
}

char* dictionary_shrink_word(
	char wordBuffer[WORD_MAX_SIZE], 
	size_t wordLength) {

    // Remove new line character and replace with null terminating
    wordBuffer[wordLength] = '\0';
	
    // Allocate memory for the word 
    char* word = (char*)malloc(sizeof(char) * (wordLength + 1));

    // Copy the original word from the buffer and into wordPtr
    memcpy(word, wordBuffer, (wordLength + 1));

    return word;
}
