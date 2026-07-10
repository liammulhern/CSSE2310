/*
 * ============================================================================
 *
 *       fileName:  validity.h
 *
 *    Description:  Header file for validity.c
 *
 *        Version:  1.0
 *        Created:  05/08/23 21:36:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern 
 *   Organization:  CSSE2310 A1 
 *
 * ============================================================================
 */
#ifndef VALIDITY_H
#define VALIDITY_H

#define DEFAULT_WORD_LENGTH 4
#define DEFAULT_STEP_LIMIT 13
#define DEFAULT_DICT_FILE "/usr/share/dict/words"
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 9
#define MAX_STEP_LIMIT 50

#include <stdio.h>

#include "arguments.h"

/* validity_check_value()
 * ---------------
 * Checks the validity of the values present within the Arguments struct.
 * 
 * Parameters:
 *  arguments: Pointer to Arguments struct that contains command
 *	line arguments and default values if not overwritten.
 *
 * Returns:
 *  The error status of the error that occured in called funcitons.
 * 
 * Errors:
 *  None.
 */
int validity_check_value(Arguments* arguments);

/* validity_check_length()
 * ---------------
 * Checks if more than one of sourceWord, finalWord, or wordLength is 
 * specified, and if the word lengths are consistent. Also checks if the 
 * length is between 2 and 9 inclusive. 
 *
 * Parameters:
 *  arguments: Pointer to Arguments struct that contains command
 *	line arguments and default values if not overwritten.
 *
 * Returns:
 *  If word lengths are inconsistent returns ERR_LEN_CONFLICT (15)
 *  If word lengths are not between 2 and 9 (inclusive) returns 
 *  ERR_LEN_RANGE (2), 0 otherwise.
 *
 * Errors:
 *  None
 */
int validity_check_length(Arguments* arguments);

/* validity_check_word_length()
 * ---------------
 * Check if word is specified (i.e len != 0), if it is check that word length
 * is consistent.
 * 
 * Parameters:
 *  wordLength: The length of the word to be checked.
 *  currentWordLength: The currently accepted word length.
 *
 * Returns:
 *  True if word length is invalid, false otherwise.
 * 
 * Errors:
 *  None
 */
bool validity_check_word_length(int wordLength, int* currentWordLength);

/* validity_set_word_default()
 * ---------------
 * If --start, --target, or --len are not specified set the 
 * DEFAULT_WORD_LENGTH and generate randome words before updating the 
 * Arguments struct.
 * 
 * Parameters:
 *  arguments: Pointer to Arguments struct that contains command
 *	line arguments and default values if not overwritten.
 *  currentWordLength: The length of the currently accepted valid word.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void validity_set_word_default(Arguments* arguments, int currentWordLength);

/* validity_check_word_length_range()
 * ---------------
 * Checks if the word length is between MIN_WORD_LENGTH and MAX_WORD_LENGTH.
 * 
 * Parameters:
 *  wordLength: The length of the word to check.
 *
 * Returns:
 *  True if the word length is outside of the range, false otherwise.
 * 
 * Errors:
 *  None
 */
bool validity_check_word_length_range(int wordLength);

/* validity_compare_length()
 * ---------------
 * Compares the length of the currently accepted word and the length
 * of an argument. 
 *
 * Parameters:
 *  wordLength: The length of the currently accepted word.
 *  argumentLength: The length of the argument word to compare.
 *
 * Returns:
 *  True if the word lengths are consistent, false otherwise.
 * 
 * Errors:
 *  None
 */
bool validity_compare_length(int wordLength, int argumentLength);

/* validity_check_word()
 * ---------------
 * Checks if sourceWord or finalWord is specified and if the arguments only 
 * contains letters. If both sourceWord and finalWord are specified checks 
 * that words are different.  
 *
 * Parameters:
 *  arguments: Pointer to Arguments struct that contains command
 *	line arguments and default values if not overwritten.
 *
 * Returns:
 *  If arguments do not contain only letters returns ERR_CMD_ARGS (9).
 *  If sourceWord and finalWord are equal returns ERR_WORD_DIFF (5).
 *  0 otherwise.
 * 
 * Errors:
 *  None
 */
int validity_check_word(Arguments* arguments);

/* validity_check_chars()
 * ---------------
 * Checks the argument characters for valid letters both upper and lowercase.
 * 
 * Parameters:
 *  argument: The word that will be checked for non letter characters.
 *
 * Returns:
 *  True if argument contains invalid characters, false otherwise.
 * 
 * Errors:
 *  None 
 */
bool validity_check_chars(char* argument);

/* validity_check_step_limit()
 * ---------------
 * If the stepLimit is specified checks that the value is between the word 
 * length and 50 (inclusive). 
 *
 * Parameters:
 *  arguments: Pointer to Arguments struct that contains command
 *	line arguments and default values if not overwritten.
 *
 * Returns:
 *  If stepLimit is not between the word length and 50 returns ERR_LIMIT_RANGE
 *  (18), 0 otherwise.
 * 
 * Errors:
 *  None
 */
int validity_check_step_limit(Arguments* arguments);

/* validity_check_dictionary()
 * ---------------
 * Check if the dictionary file argument is set, if not set to the default.
 * 
 * Parameters:
 *  arguments: Pointer to Arguments struct that contains command
 *	line arguments and default values if not overwritten.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void validity_set_dictionary_default(Arguments* arguments);

/* validity_check_dictionary_filename()
 * ---------------
 * If the dictionary fileName is specified, checks that the path exists and
 * can be opened.
 *
 * Parameters:
 *  fileName: The file path to the specified dictionary file.
 *  wordList: The file pointer that is attempting to open.
 *
 * Returns:
 *  ERR_DICT_FILE (12) if dictionary fileName does not exist, 0 otherwise.
 * 
 * Errors:
 *  None
 */
int validity_check_dictionary_filename(char* fileName, FILE* wordList);

#endif
