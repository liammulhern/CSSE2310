/*
 * ============================================================================
 *
 *       fileName:  dictionary.h
 *
 *    Description:  Header file for dictionary.c
 *
 *        Version:  1.0
 *        Created:  06/08/23 18:27:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern 
 *   Organization:  CSSE2310 A1
 *
 * ============================================================================
 */
#ifndef DICTIONARY_H
#define DICTIONARY_H

#define WORD_MAX_SIZE 51
#define WORD_WILDCARD_CHAR '_'
#define WORD_DICTIONARY_SIZE 100000
#define BUCKET_DICTIONARY_SIZE 100000
#define SUGGESTION_DICTIONARY_SIZE 100000
#define EDGE_DICTIONARY_SIZE 50

#include <stddef.h>

#include "hashtable.h"
#include "list.h"
#include "game.h"

/* dictionary_get_words()
 * ---------------
 * Reads a given file for valid words, allocates memory and 
 * returns a hashtable containing the initialised char* as keys and the 
 * index in the file as the value.
 * 
 * Parameters:
 *  fileName: The file or path to the file to read from.
 *  inputWordLength: The length of the words to be read from the dictionary.
 *
 * Returns:
 *  A hashtable containing the initialised words.
 * 
 * Errors:
 *  None
 */
HashTable* dictionary_get_words(
	char* fileName, 
	unsigned short inputWordLength);

/* dictionary_create_word_buckets()
 * ---------------
 * Creates wildcard word 'bucket' as the key for a HashTable that allows 
 * linear time comparison for each word. The value stored is then a linked 
 * list of the words that fit the pattern.
 *
 * e.g 
 * Take the word "ABCD", this is turned into the buckets:
 * "_BCD", "A_CD", "AB_D", "ABC_"
 * If one of these buckets already exists in the HashTable from a previous 
 * word, then the word "ABCD" would be added to that bucket's linked list 
 * as it meets the pattern.
 * 
 * Parameters:
 *  wordDictionary: HashTable of allocated words where the keys are char* and 
 *	values are the index in the dictionary file.
 *
 * Returns:
 *  HashTable of buckets and subsequent words that fit the pattern.
 * 
 * Errors:
 *  None
 */
HashTable* dictionary_create_word_buckets(HashTable* wordDictionary);

/* dictionary_create_word_bucket()
 * ---------------
 * Create and inserts the HashTable key/value pair that forms that bucket.
 * 
 * Parameters:
 *  bucketDictionary: HashTable of buckets and associated words that meets
 *	the bucket pattern.
 *  wordDictionary: HashTable of allocated words where the keys are char* and 
 *	values are the index in the dictionary file.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void dictionary_create_word_bucket(
	HashTable* bucketDictionary, 
	HashTableNode* wordDictionaryNode);

/* dictionary_create_word_graph()
 * ---------------
 * Create the graph HashTable where each word is a vertex and each edge is
 * a word that differs by 1 character. The HashTable key is the word and 
 * and the value is a HashTable with keys of words that differ by 1 character 
 * and values of the index in the dictionary file.
 * 
 * Parameters:
 *  bucketDictionary: HashTable of buckets and associated words that meets
 *	the bucket pattern.
 *  wordDictionary: HashTable of allocated words where the keys are char* and 
 *	values are the index in the dictionary file.
 *
 * Returns:
 *  HashTable graph of word suggestions.
 * 
 * Errors:
 *  None
 */
HashTable* dictionary_create_word_graph(
	HashTable* bucketDictionary, 
	HashTable* wordDictionary);

/* dictionary_create_word_graph_edges()
 * ---------------
 * Add the cartesian product of a given bucket's words to each word key's
 * HashTable. 
 * 
 * Parameters:
 *  suggestionDictionary: HashTable of word suggestions that represents graph. 
 *  wordDictionary: HashTable of allocated words where the keys are char* and 
 *	values are the index in the dictionary file.
 *  bucketDictionaryNode: Key value pair from the bucketDictionary where the
 *	key is a word pattern and value is words that fit that pattern.
 *	
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void dictionary_create_word_graph_edges(
	HashTable* suggestionDictionary, 
	HashTable* wordDictionary, 
	HashTableNode* bucketDictionaryNode);

/* dictionary_create_word_graph_edge()
 * ---------------
 * Insert edge words into suggestionDictionary where the key is the wordKey 
 * to connect the vertices of graph.
 * 
 * Parameters:
 *  suggestionDictionary: HashTable of word suggestions that represents graph. 
 *  wordDictionary: HashTable of allocated words where the keys are char* and 
 *	values are the index in the dictionary file.
 *  wordKey: The word that is 1 character different from all edge words.
 *  edgeWord: The word that will be added to the wordKey's HashTable.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void dictionary_create_word_graph_edge(
	HashTable* suggestionDictionary, 
	HashTable* wordDictionary, 
	char* wordKey, 
	char* edgeWord);

/* dictionary_get_suggestions()
 * ---------------
 * Get the sorted suggestions in the form of a linked list for a given word 
 * from the suggestionDictionary.
 * 
 * Parameters:
 *  gameState: The current state of the game, including pointers to the 
 *	arguments, wordDictionary, suggestionDictionary, previousWords, 
 *	currentWord, and currentSteps.
 *
 * Returns:
 *  Linked list of suggested words sorted by file index.
 * 
 * Errors:
 *  None
 */
List* dictionary_get_suggestions(GameState* gameState);
	
/* dictionary_get_suggestions_table()
 * ---------------
 * Get the HashTable of suggested words for any given word.
 * 
 * Parameters:
 *  wordDictionary: HashTable of allocated words where the keys are char* and 
 *	values are the index in the dictionary file.
 *  suggestionDictionary: HashTable of word suggestions that represents graph. 
 *  bucketDictionary: HashTable of word patterns used if word is not in 
 *	graph.
 *  word: The word to find suggestions for.
 *  freeFlag: Output pointer set if HashTable needs to be freed.
 *
 * Returns:
 *  HashTable of suggested words for any given word and sets freeFlag if 
 *  returned value requires freeing.
 * 
 * Errors:
 *  None
 */
HashTable* dictionary_get_suggestions_table(
	HashTable* wordDictionary, 
	HashTable* suggestionDictionary, 
	HashTable* bucketDictionary,
	char* word,
	int* freeFlag);

/* dictionary_get_suggestions_table_unknown()
 * ---------------
 * Create a HashTable of suggested words from a given unknown word.
 * 
 * Parameters:
 *  wordDictionary: HashTable of allocated words where the keys are char* and 
 *	values are the index in the dictionary file.
 *  suggestionDictionary: HashTable of word suggestions that represents graph. 
 *  bucketDictionary: HashTable of word patterns used if word is not in 
 *	graph.
 *  word: The word to find suggestions for.
 *
 * Returns:
 *  Table of suggested words for the given unkown word. 
 * 
 * Errors:
 *  None
 */
HashTable* dictionary_get_suggestions_table_unknown(
	HashTable* wordDictionary, 
	HashTable* suggestionDictionary, 
	HashTable* bucketDictionary,
	char* word);

/* dictionary_create_suggestions_table_unknown()
 * ---------------
 * Insert word suggestions into suggestionsTable for an unknown word that 
 * is not in the word list.
 * 
 * Parameters:
 *  wordDictionary: HashTable of allocated words where the keys are char* and 
 *	values are the index in the dictionary file.
 *  suggestionsTable: The table of similar words to insert the suggestions 
 *	from the word bucket into.
 *  wordBucket: The list of similar words to insert into the 
 *	suggestionsTable.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void dictionary_create_suggestions_table_unknown(
	HashTable* wordDictionary,
	HashTable* suggestionsTable, 
	List* wordBucket);

/* dictionary_create_suggestions_array()
 * ---------------
 * Convert a list of long* to an array of longs of the same size.
 * 
 * Parameters:
 *  list: The list to convert.
 *
 * Returns:
 *  The array of dereferenced longs.
 * 
 * Errors:
 *  None
 */
long* dictionary_create_suggestions_array(List* list);

/* dictionary_sort_suggestions_array()
 * ---------------
 * Sort the array of longs from smallest to largest using the quick sort 
 * algorithm and overwrite the given array.
 * 
 * Parameters:
 *  suggestionsIndex: The array of longs to be sorted.
 *  size: The length of the array.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void dictionary_sort_suggestions_array(long* suggestionsIndex, size_t size);

/* dictionary_create_suggestions_index_list()
 * ---------------
 * Create a linked list of the file number indices for each word in the 
 * suggestionsTable.
 * 
 * Parameters:
 *  suggestionsTable: HashTable with file index as the key and suggested word
 *	as the value.
 *  previousWords: The list of words entered previously in the current game. 
 *  finalWord: The final target word in the current game.
 *  finalWordFlag: Flag set if the final word is in the list of 
 *	suggested words.
 *
 * Returns:
 *  Linked list of long* file indices and set finalWordFlag if final word is
 *  in the list of suggested words.
 * 
 * Errors:
 *  None
 */
List* dictionary_create_suggestions_index_list(
	HashTable* suggestionsTable,
	List* previousWords,
	char* finalWord, 
	int* finalWordFlag);

/* dictionary_create_suggestions_list()
 * ---------------
 * Create the linked list of sorted word suggestions using the 
 * suggestionsIndex.
 * 
 * Parameters:
 *  suggestionsTable: HashTable with file index as the key and suggested word
 *	as the value.
 *  suggestionsIndexArray: The sorted array of longs that will be used as keys.
 *  suggestionsIndexSize: The size of the suggestionsIndex array.
 *  finalWord: The final target word in the current game.
 *  finalWordFlag: Flag set if the final word is in the list of 
 *	suggested words.
 *
 * Returns:
 *  Linked list of sorted word suggestions.
 * 
 * Errors:
 *  None
 */
List* dictionary_create_suggestions_list(
	HashTable* suggestionsTable, 
	long* suggestionsIndex,
	size_t suggestionsIndexSize,
	char* finalWord, 
	int finalWordFlag);

/* dictionary_compare_suggestions_index()
 * ---------------
 * Compares 2 long indices from the suggestionsIndexArray to sort.
 * 
 * Parameters:
 *  index1: The first long to compare.
 *  index2: The second long to compare.
 *
 * Returns:
 *  True if the first long is greater than the second. False otherwise.
 * 
 * Errors:
 *  None
 */
int dictionary_compare_suggestions_index(
	const void* index1, 
	const void* index2);

/* dictionary_check_word_validity()
* ---------------
* Checks if a word contains invalid characters and capitalises each character.
*
* Parameters:
*  wordBuffer: The buffer of size WORD_MAX_SIZE that the string is stored in.
*  wordLength: The number of characters in the string.
*  inputWordLength: The length of the required words.
*
* Returns:
*  True if the word is valid and false otherwise.
* 
* Errors:
*  None
*/
int dictionary_check_word_validity(
	char wordBuffer[WORD_MAX_SIZE], 
	size_t wordLength, 
	unsigned short inputWordLength);

/* dictionary_shrink_word()
 * ---------------
 * Shrinks a string contained in an input buffer of size WORD_MAX_SIZE to
 * it respective length dictated by the new line character and terminates
 * it with a null character. A pointer to the allocated memory is returned. 
 * 
 * Parameters:
 *  wordBuffer: The buffer of size WORD_MAX_SIZE that the string is stored in.
 *  wordLength: The number of characters in the string.
 *
 * Returns:
 *  A pointer to the allocated memory for the shrunk word. 
 * 
 * Errors:
 *  None
 */
char* dictionary_shrink_word(
	char wordRaw[WORD_MAX_SIZE],
	size_t wordLength);

#endif
