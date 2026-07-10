/*
 * ============================================================================
 *
 *       fileName:  game.c
 *
 *    Description:  Implements program operation and user input from the 
 *		    terminal.
 *
 *        Version:  1.0
 *        Created:  16/08/23 10:45:06
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
#include <ctype.h>

#include "game.h"
#include "validity.h"
#include "error.h"
#include "dictionary.h"

void game_start(int argc, char* argv[]) {
    Arguments* arguments = game_get_arguments(argc, argv);
    HashTable* wordDictionary = game_get_words(arguments);
    HashTable* bucketDictionary = game_get_buckets(wordDictionary);
    HashTable* suggestionDictionary = game_get_suggestions(wordDictionary, 
	    bucketDictionary);
    List* previousWords = game_get_previous_words(arguments);

    GameState* gameState = game_create_state(arguments, wordDictionary, 
	    bucketDictionary, suggestionDictionary, previousWords);

    game_play(gameState);
}

Arguments* game_get_arguments(int argc, char* argv[]) {
    Arguments* arguments = arguments_get(argc, argv);
    int argumentsError = validity_check_value(arguments);

    // Check if errors occured when validating arguments
    if (argumentsError) {
	arguments_destroy(arguments);
	error_exit(argumentsError);
    }
    
    return arguments;
}

HashTable* game_get_words(Arguments* arguments) {
    int wordLength = arguments->wordLength;
    char* fileName = arguments->fileName;

    HashTable* wordDictionary = dictionary_get_words(fileName, wordLength);

    if (wordDictionary == NULL) {
	char* fileName = strdup(arguments->fileName);
	arguments_destroy(arguments);
	error_dictionary_file_invalid(fileName);
    }

    return wordDictionary;
}

HashTable* game_get_buckets(HashTable* wordDictionary) {
    HashTable* bucketDictionary = dictionary_create_word_buckets(
	    wordDictionary);

    return bucketDictionary;
}

HashTable* game_get_suggestions(
	HashTable* wordDictionary, 
	HashTable* bucketDictionary) {

    HashTable* suggestionDictionary = dictionary_create_word_graph(
	    bucketDictionary, wordDictionary);

    return suggestionDictionary;
}

List* game_get_previous_words(Arguments* arguments) {
    List* previousWords = list_create(free, print_string_pointer);
    list_append(previousWords, strdup(arguments->sourceWord));

    return previousWords;
}

GameState* game_create_state(
	Arguments* arguments, 
	HashTable* wordDictionary,
	HashTable* bucketDictionary,
	HashTable* suggestionDictionary,
	List* previousWords) {
    GameState* gameState = (GameState*) malloc(sizeof(GameState));
    
    gameState->arguments = arguments;
    gameState->wordDictionary = wordDictionary;
    gameState->bucketDictionary = bucketDictionary;
    gameState->suggestionDictionary = suggestionDictionary;
    gameState->previousWords = previousWords;
    gameState->currentWord = arguments->sourceWord;
    gameState->currentSteps = INITAL_STEPS;

    return gameState;
}

void game_update_state(
	GameState* gameState, 
	char inputBuffer[INPUT_BUFFER_SIZE]) {
	
    gameState->currentWord = strdup(inputBuffer);
    list_append(gameState->previousWords, gameState->currentWord);
}

void game_play(GameState* gameState) {
    game_prompt_start(gameState->arguments);

    char inputBuffer[INPUT_BUFFER_SIZE];

    while (true) {
	game_prompt_input(gameState->currentSteps);

	// Get input from stdin and store in inputBuffer
	if (game_get_input(inputBuffer)) {
	    game_over_eof(gameState);
	    break;
	}
	
	// Check if input is "?" for suggestion 
	if (strcmp(inputBuffer, SUGGESTION) == 0) {
	    game_suggestions(gameState);
	    continue;
	}

	// Check if input is valid word 
	if (game_check_input_validity(inputBuffer, gameState)) {
	    continue;
	} 

    	game_update_state(gameState, inputBuffer);
	game_check_game_over(gameState);
	gameState->currentSteps++;
    }
}

void game_suggestions(GameState* gameState) {
    List* suggestions = dictionary_get_suggestions(gameState);
    
    // Check if suggestions are available for the current word 
    if (suggestions == NULL || suggestions->size == 0) {
	game_prompt_no_suggestions();
    } else {
	game_prompt_suggestions(suggestions);
    }

    list_destroy(suggestions);
}

void game_check_game_over(GameState* gameState) {
    // Check current step count
    if (gameState->arguments->stepLimit <= gameState->currentSteps) {
	game_over(gameState);
    }
    
    // Check if current word matches the final word 
    if (strcmp(gameState->arguments->finalWord, 
	    gameState->currentWord) == 0) {
	game_win(gameState);
    }
}

void game_over(GameState* gameState) {
    game_prompt_game_over_attempts();
    game_destroy(gameState);
    error_game_over();
}

void game_over_eof(GameState* gameState) {
    game_prompt_game_over();
    game_destroy(gameState);
    error_end_of_file();
}

void game_win(GameState* gameState) {
    game_prompt_game_win(gameState->currentSteps);
    game_destroy(gameState);
    error_win();
}

void game_destroy(GameState* gameState) {
    if (gameState->previousWords != NULL) {
	list_destroy(gameState->previousWords);
    }

    if (gameState->arguments != NULL) {
	arguments_destroy(gameState->arguments);
    }
    
    if (gameState->suggestionDictionary != NULL) {
	hash_table_destroy(gameState->suggestionDictionary);
    }

    if (gameState->bucketDictionary != NULL) {
	hash_table_destroy(gameState->bucketDictionary);
    }

    if (gameState->wordDictionary != NULL) {
	hash_table_destroy(gameState->wordDictionary);
    }

    free(gameState);
}

int game_get_input(char inputBuffer[INPUT_BUFFER_SIZE]) {
    // Read stdin into the input buffer if it exists  
    if (fgets(inputBuffer, sizeof(char) * INPUT_BUFFER_SIZE, stdin) == NULL) {
	return ERR_EOF;
    }
    
    // Initial length includes escape characters
    int inputLength = strnlen(inputBuffer, INPUT_BUFFER_SIZE);
    
    if (inputBuffer[inputLength - 1] != '\n') {
	int overflow;   

	while (((overflow = getchar()) != '\n') && (overflow != EOF)) {
	    // Flush the input until newline or EOF to clear stdin buffer.
	}
	
	// Add null terminating character to end of string not containing CR.
	inputBuffer[inputLength] = '\0';
    } else {
	// Add null terminating character to string by replacing CR.
	inputBuffer[inputLength - 1] = '\0';
    }

    return 0;
}

bool game_check_input_validity(
	char inputBuffer[INPUT_BUFFER_SIZE],
	GameState* gameState) {

    // Check input buffer validity in order stated in specification
    return (game_check_input_length(inputBuffer, gameState->arguments)
	    || game_check_input_word(inputBuffer)
	    || game_check_input_word_difference(inputBuffer, 
		gameState->previousWords)
	    || game_check_input_word_previous(inputBuffer, 
		    gameState->previousWords)
	    || game_check_input_word_dictionary(inputBuffer, 
		    gameState->wordDictionary));
}

bool game_check_input_length(
	char inputBuffer[INPUT_BUFFER_SIZE],
	Arguments* arguments) {

    int wordLength = strlen(inputBuffer);

    // Check if the input word length matches the required length.
    if (arguments->wordLength != wordLength) {
	game_prompt_invalid_length(arguments->wordLength);
	return true;
    }

    return false;
}

bool game_check_input_word(char inputBuffer[INPUT_BUFFER_SIZE]) {
    int inputLength = strlen(inputBuffer);

    // Check if any characters are not letters and exit if any are found
    for (int i = 0; i < inputLength; i++) {
	if (!isalpha(inputBuffer[i])) {
	    game_prompt_invalid_char();
	    return true;
	} else {
	    // Capitalise argument character
	    inputBuffer[i] = toupper(inputBuffer[i]);
	}
    }

    return false;
}

bool game_check_input_word_difference(
	char inputBuffer[INPUT_BUFFER_SIZE], 
	List* previousWords) { 

    // Previous word is the last element of the linked list 
    char* previousWord = previousWords->tail->object;
    int inputLength = strlen(inputBuffer);
    int inputDifference = 0;

    for (int i = 0; i < inputLength; i++) {
	// Check character of input is different to the word entered 
	// previously
	if (previousWord[i] != inputBuffer[i]) {
	    inputDifference++;

	    if (inputDifference > WORD_CHAR_DIFF) {
		break;
	    }
	}
    }

    if (inputDifference != WORD_CHAR_DIFF) {
	game_prompt_invalid_difference();
	return true;
    }   

    return false;
}

bool game_check_input_word_previous(
	char inputBuffer[INPUT_BUFFER_SIZE], 
	List* previousWords) {

    // Find index of input buffer in previous word list if string matches
    int index = list_find(previousWords, inputBuffer, list_compare_char);

    // Check if word has already been entered in the game 
    if (index != NOT_IN_LIST) {
	game_prompt_invalid_previous();
	return true;
    } 

    return false;
}

bool game_check_input_word_dictionary(
	char inputBuffer[INPUT_BUFFER_SIZE],
	HashTable* wordDictionary) {
    
    // Find if key exists in HashTable when comparing strings
    if (!hash_table_key_exists_compare(wordDictionary, inputBuffer, 
	    strlen(inputBuffer), hash_table_compare_char)) {
	game_prompt_invalid_dictionary();
	return true;
    }

    return false;
}

void game_prompt_start(Arguments* arguments) {
    printf("Welcome to UQWordLadder!\n");
    printf("Your goal is to turn '%s' into '%s' in at most %d steps\n", 
	    arguments->sourceWord, arguments->finalWord, arguments->stepLimit);
}

void game_prompt_input(unsigned short currentSteps) {
    printf("Enter word %d (or ? for help):\n", currentSteps);
}

void game_prompt_no_suggestions() {
    printf("No suggestions found.\n");
}

void game_prompt_suggestions(List* suggestions) {
    printf("Suggestions:-----------\n");
    
    size_t index = 0;
    ListNode* currentNode = NULL;

    // Iterate over list nodes and convert objects to char* before printing
    while ((currentNode = list_iterate(suggestions, currentNode, &index)) 
	    != NULL) {
	char* suggestion = (char*) currentNode->object;
	printf(" %s\n", suggestion);
    }

    printf("-----End of Suggestions\n");
}

void game_prompt_invalid_length(int wordLength) {
    printf("Word should be %d characters long - try again.\n", wordLength);
}

void game_prompt_invalid_char() {
    printf("Word contains non-letter characters - try again.\n");
}

void game_prompt_invalid_difference() {
    printf("Word must differ by only one letter from previous "
	    "word - try again.\n");
}

void game_prompt_invalid_previous() {
    printf("You cannot return to a previous word - try again.\n");
}

void game_prompt_invalid_dictionary() {
    printf("Word is not in dictionary - try again.\n");
}

void game_prompt_game_over() {
    printf("Game over - you gave up.\n");
}

void game_prompt_game_over_attempts() {
    printf("Game over - no more attempts remaining.\n");
}

void game_prompt_game_win(unsigned short currentSteps) {
    printf("Congratulations - you solved the ladder in %d steps.\n", 
	    currentSteps);
}
