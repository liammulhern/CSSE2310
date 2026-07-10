/*
 * ============================================================================
 *
 *       fileName:  validity.c
 *
 *    Description:  Checks the validity of values after the command line
 *		    validity has been checked.
 *
 *        Version:  1.0
 *        Created:  05/08/23 20:52:16
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
#include <ctype.h>
#include <csse2310a1.h>

#include "validity.h"
#include "error.h"

int validity_check_value(Arguments* arguments) {
    int lengthError = validity_check_length(arguments);
    if (lengthError) {
	return lengthError;
    }

    int wordError = validity_check_word(arguments);
    if (wordError) {
	return wordError;
    }
    
    int limitError = validity_check_step_limit(arguments);
    if (limitError) {
	return limitError;
    }

    validity_set_dictionary_default(arguments);

    return 0;
}

int validity_check_length(Arguments* arguments) {
    int currentWordLength = 0;

    // If wordLengthError changes from 0 an error has occured.
    int wordLengthError = 0;

    wordLengthError += validity_check_word_length(
	    strlen(arguments->sourceWord), &currentWordLength);

    wordLengthError += validity_check_word_length(
	    strlen(arguments->finalWord), &currentWordLength);
    
    wordLengthError += validity_check_word_length(arguments->wordLength, 
	    &currentWordLength);

    if (wordLengthError != 0) {
	return ERR_LEN_CONFLICT;
    }
    
    // If wordLength is 0 set it to DEFAULT_WORD_LENGTH
    if (!currentWordLength) {
	currentWordLength = DEFAULT_WORD_LENGTH; 
    }

    if (validity_check_word_length_range(currentWordLength)) {
	return ERR_LEN_RANGE;
    }

    validity_set_word_default(arguments, currentWordLength);

    return 0;
}

bool validity_check_word_length(int wordLength, int* currentWordLength) {
    // If word length is not default value, check if length is consistent.
    if (wordLength) {
	if (validity_compare_length(*currentWordLength, wordLength)) {
	    *currentWordLength = wordLength;
	    return false;
	}
	return true;
    }
    return false;
}

void validity_set_word_default(Arguments* arguments, int currentWordLength) {
    arguments->wordLength = currentWordLength;
    
    // If the sourceWord is not specified generate random
    if (!arguments->sourceWord[0]) {
	const char* randomWord = get_uqwordladder_word(currentWordLength);
	arguments->sourceWord = strdup(randomWord);
    }

    // If the finalWord is not specified generate random
    if (!arguments->finalWord[0]) {
	const char* randomWord = get_uqwordladder_word(currentWordLength);
	arguments->finalWord = strdup(randomWord);
    }
}

bool validity_check_word_length_range(int wordLength) {
    return wordLength < MIN_WORD_LENGTH || wordLength > MAX_WORD_LENGTH;
}

bool validity_compare_length(int wordLength, int argumentLength) {
    return !wordLength || (wordLength && argumentLength == wordLength);
}

int validity_check_word(Arguments* arguments) {
    char* sourceWord = arguments->sourceWord;
    char* finalWord = arguments->finalWord;

    int wordCharError = 0;
    wordCharError += validity_check_chars(sourceWord);
    wordCharError += validity_check_chars(finalWord);

    if (wordCharError) {
	return ERR_WORD_CHARS;
    }

    // If sourceWord and finalWord are specified check difference.
    if (sourceWord[0] && finalWord[0] && !strcmp(sourceWord, finalWord)) {
	return ERR_WORD_DIFF;
    }

    return 0;
}

bool validity_check_chars(char* argument) {
    // Check if argument is specified
    if (argument[0]) {
	int argumentLength = strlen(argument);

	// Check if any characters are not letters and exit if any are found
	for (int i = 0; i < argumentLength; i++) {
	    if (!isalpha(argument[i])) {
		return true;
	    } else {
		// Capitalise argument character
		argument[i] = toupper(argument[i]);
	    }
	}
    }

    return false;
}

int validity_check_step_limit(Arguments* arguments) {
    // If stepLimit is not default value and is not between the word length
    // and MAX_STEP_LIMIT print prompt and exit.
    int stepLimit = arguments->stepLimit; 
    int wordLength = arguments->wordLength;
   
    if (stepLimit && (stepLimit < wordLength || stepLimit > MAX_STEP_LIMIT)) {
	return ERR_LIMIT_RANGE;
    } else if (!stepLimit) {
	arguments->stepLimit = DEFAULT_STEP_LIMIT;
    }

    return 0;
}

void validity_set_dictionary_default(Arguments* arguments) {
    // If dictionary file name is not specified set to default
    if (!(arguments->fileName[0])) {
	arguments->fileName = strdup(DEFAULT_DICT_FILE);
    }
}

int validity_check_dictionary_filename(char* fileName, FILE* wordList) {
    if (wordList == NULL) {
	return ERR_DICT_FILE;
    }  

    return 0;
}
