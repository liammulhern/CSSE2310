/*
 * ============================================================================
 *
 *       fileName:  game.h
 *
 *    Description:  Header file for game.c
 *
 *        Version:  1.0
 *        Created:  16/08/23 10:47:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern 
 *   Organization:  CSSE2310 A1
 *
 * ============================================================================
 */
#ifndef GAME_H
#define GAME_H

#define INITAL_STEPS 1
#define WORD_CHAR_DIFF 1 
#define INPUT_BUFFER_SIZE 50
#define SUGGESTION "?"

#include <stdbool.h>

#include "arguments.h"
#include "list.h"
#include "hashtable.h"

/* GameState
 * ---------------
 * Struct representing the game state using the information 
 * given by the user.
 * 
 * Parameters:
 *  arguments: The valid Arguments from the terminal.
 *  wordDictionary: The dictionary containing valid words of the correct 
 *	length for the current game.
 *  bucketDictionary: The dictionary of word patterns used to create the 
 *	suggestionDictionary and find suggestions for unknown words.
 *  suggestionDictionary: The dictionary of suggestions for a given word.
 *  previousWords: The list of word previously entered in the game.
 *  currentWord: The current word in play.
 *  currentSteps: The current number of steps in the game so far.
 */
typedef struct GameState {
    Arguments* arguments;
    HashTable* wordDictionary;
    HashTable* bucketDictionary;
    HashTable* suggestionDictionary;
    List* previousWords;
    char* currentWord;
    unsigned short currentSteps;
} GameState;

/* game_start()
 * ---------------
 * Entry point to UQWordLadder that creates GameState after getting 
 * required game information.
 * 
 * Parameters:
 *  argc: The argument count passed to the main function from the console.
 *  argv: The argument array passed to the main function from the console.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits program via error.c if arguments or word list is invalid.
 */
void game_start(int argc, char* argv[]);

/* game_get_arguments()
 * ---------------
 * Get pointer to terminal arguments and validate before returning pointer 
 * containing sourceWord, finalWord, wordLength, stepLimit, and fileName.
 *
 * Freed if error occurs or when GameState struct is destoryed.
 * 
 * Parameters:
 *  argc: The argument count passed to the main function from the console.
 *  argv: The argument array passed to the main function from the console.
 *
 * Returns:
 *  Arguments struct containing sourceWord, finalWord, wordLength, 
 *  stepLimit, and fileName.
 * 
 * Errors:
 *  Exits if argument error is found via error.c
 */
Arguments* game_get_arguments(int argc, char* argv[]);

 /* game_get_words()
 * ---------------
 * Get the dictionary words read from the dictionary file as a HashTable where
 * the key is the valid word capitalised and the value is its order in the 
 * original dictionary file.
 * 
 * e.g:
 * {
 *  ...
 *  ["WORD"] : [4013]
 *  ...
 * }
 *
 * Freed if error occurs or when GameState struct is destoryed.
 *
 * Parameters:
 *  arguments: Arguments struct that contains valid command
 *	line arguments and/or default values.
 *
 * Returns:
 *  HashTable where the key is the valid word capitalised and the value 
 *  is its order in the original dictionary file.
 * 
 * Errors:
 *  Exits if dictionary file cannot be read via error.c
 */
HashTable* game_get_words(Arguments* arguments);

/* game_get_buckets
 * ---------------
 * Get the word buckets for all words in the dictionary as a HashTable where
 * the key is a word pattern and the value is a list of all words in the 
 * wordDictionary that meet the pattern.
 * 
 * Parameters:
 *  wordDictionary: HashTable of valid words from the dictionary file.
 *
 * Returns:
 *  HashTable where the key is a word pattern and the value is a list 
 *  of all words in the wordDictionary that meet the pattern.
 * 
 * Errors:
 *  None
 */
HashTable* game_get_buckets(HashTable* wordDictionary);

/* game_get_suggestions()
 * ---------------
 * Get the suggestions for all words in the dictionary as a HashTable where
 * the key is a valid word from the dictionary file and the value is another
 * HashTable where the key is the similar words order in the file and value
 * is the word. 
 *
 * e.g:
 * {
 *  ...
 *  ["WORD"] : {[4512] : ["WARD"], [1013] : ["HORD"], ...},
 *  ...
 * }
 *
 * Freed when GameState struct is destoryed.
 *
 * Parameters:
 *  wordDictionary: HashTable of valid words from the dictionary file.
 *  bucketDictionary: HashTable of word pattern key and list of similar
 *	words as values.
 *
 * Returns:
 *  HashTable where the key is a valid word from the dictionary file and 
 *  the value is another HashTable where the key is the similar words 
 *  order in the file and value is the word. 
 * 
 * Errors:
 *  None
 */
HashTable* game_get_suggestions(
	HashTable* wordDictionary, 
	HashTable* bucketDictionary);

/* game_get_previous_words()
 * ---------------
 * Get the linked list of previous words that have occured in the game.
 * Initialised with the sourceWord.
 * 
 * Free when GameState struct is destoryed.
 *
 * Parameters:
 *  arguments: Arguments struct that contains valid command
 *	line arguments and/or default values.
 *
 * Returns:
 *  Linked list of previous words.
 * 
 * Errors:
 *  None
 */
List* game_get_previous_words(Arguments* arguments);

/* game_create_state()
 * ---------------
 * Create the initial game state using the information given by the user. 
 *
 * Freed when exit status is returned from game play loop.
 * 
 * Parameters:
 *  arguments: The valid Arguments returned by game_get_arguments()
 *  wordDictionary: The valid HashTable returned by game_get_words()
 *  suggestionDictionary: The valid HashTable returned by 
 *	game_get_suggestions()
 *  previousWords: The valid List returned by game_get_previous_words()
 *
 * Returns:
 *  Initialised GameState struct.
 * 
 * Errors:
 *  None
 */
GameState* game_create_state(
	Arguments* arguments, 
	HashTable* wordDictionary,
	HashTable* bucketDictionary,
	HashTable* suggestionDictionary,
	List* previousWords);

 /* game_update_state()
 * ---------------
 * Update the GameState's currentWord and previous words list using
 * valid input.
 * 
 * Parameters:
 *  gameState: The current state of the game loop that will be update.
 *  inputBuffer: The valid input buffer that will update the state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void game_update_state(
	GameState* gameState, 
	char inputBuffer[INPUT_BUFFER_SIZE]);

/* game_play()
 * ---------------
 * Game play loop that prompts user for input, checks input, and updates the
 * game state until an exit condition is reached.
 *
 * Parameters:
 *  gameState: The current state of the game, including pointers to the 
 *	arguments, wordDictionary, suggestionDictionary, previousWords, 
 *	currentWord, and currentSteps.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  ERR_WIN (0): Current word matches the target word.
 *	(Called via game_check_game_over() -> game_win())
 *  ERR_GAME_OVER (4): Step limit is reached.
 *	(Called via game_check_game_over() -> game_over())
 *  ERR_EOF (6): EOF is detected in stdin.
 *	(Called via game_over_eof())
 */
void game_play(GameState* gameState);

/* game_suggestions()
 * ---------------
 * Prints list of suggestions to stdout for the current word.
 * 
 * Parameters:
 *  gameState: The current state of the game, including pointers to the 
 *	arguments, wordDictionary, suggestionDictionary, previousWords, 
 *	currentWord, and currentSteps.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void game_suggestions(GameState* gameState);

/* game_check_game_over()
 * ---------------
 * Check if game state has reached an exit condition.
 * 
 * Parameters:
 *  gameState: The current state of the game, including pointers to the 
 *	arguments, wordDictionary, suggestionDictionary, previousWords, 
 *	currentWord, and currentSteps.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  ERR_WIN (0): Current word matches the target word.
 *	(Called via game_win())
 *  ERR_GAME_OVER (4): Step limit is reached.
 *	(Called via game_over())
 */
void game_check_game_over(GameState* gameState);

/* game_over()
 * ---------------
 * Handle game memory freeing and prompting for game over state before 
 * exiting with ERR_GAME_OVER (4).
 * 
 * Parameters:
 *  gameState: The current state of the game, including pointers to the 
 *	arguments, wordDictionary, suggestionDictionary, previousWords, 
 *	currentWord, and currentSteps.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_GAME_OVER (4) via error.c
 */
void game_over(GameState* gameState);

/* game_over_eof()
 * ---------------
 * Handle game memory freeing and prompting for game over end of file state 
 * before exiting with ERR_EOF (6).
 * 
 * Parameters:
 *  gameState: The current state of the game, including pointers to the 
 *	arguments, wordDictionary, suggestionDictionary, previousWords, 
 *	currentWord, and currentSteps.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_EOF (6) via error.c
 */
void game_over_eof(GameState* gameState);

/* game_win()
 * ---------------
 * Handle game memory freeing and prompting for game win state 
 * before exiting with ERR_WIN (0).
 * 
 * Parameters:
 *  gameState: The current state of the game, including pointers to the 
 *	arguments, wordDictionary, suggestionDictionary, previousWords, 
 *	currentWord, and currentSteps.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_EOF (0) via error.c
 */
void game_win(GameState* gameState);

/* game_destroy()
 * ---------------
 * Destroy game state and free allocated memory for all internal references.
 * 
 * Parameters:
 *  gameState: The current state of the game, including pointers to the 
 *	arguments, wordDictionary, suggestionDictionary, previousWords, 
 *	currentWord, and currentSteps.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void game_destroy(GameState* gameState);

/* game_get_input()
 * ---------------
 * Get user input from stdin and read into inputBuffer after adding null 
 * terminating character.
 * 
 * Parameters:
 *  inputBuffer: The inputBuffer able to store INPUT_BUFFER_SIZE characters.
 *
 * Returns:
 *  Error status if input error occured, 0 otherwise.
 * 
 * Errors:
 *  None
 */
int game_get_input(char inputBuffer[INPUT_BUFFER_SIZE]);

/* game_check_input_validity()
 * ---------------
 * Check the validity of the input buffer by checking word length, invalid 
 * characters, character difference, previous matches, and if it is in the 
 * dictionary. Returns true if any checks fail, false otherwise.
 * 
 * Parameters:
 *  gameState: The current state of the game, including pointers to the 
 *	arguments, wordDictionary, suggestionDictionary, previousWords, 
 *	currentWord, and currentSteps.
 *  inputBuffer: The inputBuffer able to store INPUT_BUFFER_SIZE characters.
 *
 * Returns:
 *  True if any checks fail, false otherwise.
 * 
 * Errors:
 *  None
 */
bool game_check_input_validity(
	char inputBuffer[INPUT_BUFFER_SIZE],
	GameState* gameState);

/* game_check_input_length()
 * ---------------
 * Check if the number of characters in the inputBuffer matches the 
 * number specified in the arguments. Returns true if invalid length, false
 * otherwise.
 * 
 * Parameters:
 *  inputBuffer: The inputBuffer able to store INPUT_BUFFER_SIZE characters.
 *  arguments: Arguments struct that contains valid command
 *	line arguments and/or default values.
 *
 * Returns:
 *  True if invalid length, false otherwise.
 * 
 * Errors:
 *  None
 */
bool game_check_input_length(
	char inputBuffer[INPUT_BUFFER_SIZE],
	Arguments* arguments);

/* game_check_input_word()
 * ---------------
 * Check if the word in the inputBuffer contains invalid characters and 
 * capitalise. Returns true if invalid characters are present, false
 * otherwise.
 * 
 * Parameters:
 *  inputBuffer: The inputBuffer able to store INPUT_BUFFER_SIZE characters.
 *
 * Returns:
 *  True if invalid characters are present, false otherwise.
 *
 * Errors:
 *  None
 */
bool game_check_input_word(char inputBuffer[INPUT_BUFFER_SIZE]);

/* game_check_input_word_difference()
 * ---------------
 * Check if word in inputBuffer is WORD_CHAR_DIFF (1) characters different 
 * from the previousWord. Returns true if character difference is 
 * not WORD_CHAR_DIFF, false otherwise.
 * 
 * Parameters:
 *  inputBuffer: The inputBuffer able to store INPUT_BUFFER_SIZE characters.
 *  previousWords: The list of previousWords for the current game. 
 *
 * Returns:
 *  True if character difference is not WORD_CHAR_DIFF (1), false otherwise.
 * 
 * Errors:
 *  None
 */
bool game_check_input_word_difference(
	char inputBuffer[INPUT_BUFFER_SIZE], 
	List* previousWords);

/* game_check_input_word_previous()
 * ---------------
 * Check if the word in the inputBuffer has been entered previously. Returns
 * true if word has been entered previously, false otherwise.
 * 
 * Parameters:
 *  inputBuffer: The inputBuffer able to store INPUT_BUFFER_SIZE characters.
 *  previousWords: The list of previousWords for the current game. 
 *
 * Returns:
 *  True if word has been entered previously, false otherwise.
 * 
 * Errors:
 *  None
 */
bool game_check_input_word_previous(
	char inputBuffer[INPUT_BUFFER_SIZE], 
	List* previousWords);

/* game_check_input_word_dictionary()
 * ---------------
 * Check if the word in the inputBuffer is in the wordDictionary. Returns true
 * if word is not in the HashTable, false otherwise.
 * 
 * Parameters:
 *  inputBuffer: The inputBuffer able to store INPUT_BUFFER_SIZE characters.
 *  wordDictionary: The HashTable to check for the word.
 *
 * Returns:
 *  True if word is not in the HashTable, false otherwise.
 * 
 * Errors:
 *  None
 */
bool game_check_input_word_dictionary(
	char inputBuffer[INPUT_BUFFER_SIZE], 
	HashTable* wordDictionary);

/* game_prompt_start()
 * ---------------
 * Prompt shown when the game is first executed.
 * 
 * Parameters:
 *  arguments: Arguments struct that contains valid command
 *	line arguments and/or default values.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void game_prompt_start(Arguments* arguments);

/* game_prompt_input()
 * ---------------
 * Prompt shown at the start of each turn in the game loop.
 * 
 * Parameters:
 *  currentSteps: The current number of steps taken in the game. 
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void game_prompt_input(unsigned short steps);

/* game_prompt_no_suggestions()
 * ---------------
 * Prompt shown when no suggestions are avaliable.
 * 
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void game_prompt_no_suggestions();

/* game_prompt_suggestions()
 * ---------------
 * Prompt shown when user calls for suggestions and displays list of 
 * valid words.
 * 
 * Parameters:
 *  suggestions: List of valid suggestions to display.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void game_prompt_suggestions(List* suggestions);

/* game_prompt_invalid_length()
 * ---------------
 * Prompt shown when a word contains an invalid amount of characters.
 * 
 * Parameters:
 *  wordLength: The length of the valid number of characters.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void game_prompt_invalid_length(int wordLength);

/* game_prompt_invalid_char()
 * ---------------
 * Prompt shown when a word contains invalid characters. 
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void game_prompt_invalid_char();

/* game_prompt_invalid_difference()
 * ---------------
 * Prompt shown when a word does not differ by 1 character.
 * 
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void game_prompt_invalid_difference();

/* game_prompt_invalid_previous()
 * ---------------
 * Prompt shown when a word is repeated previously.
 * 
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void game_prompt_invalid_previous();

/* game_prompt_invalid_dictionary()
 * ---------------
 * Prompt shown when word is not present in the wordDictionary.
 * 
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void game_prompt_invalid_dictionary();

/* game_prompt_invalid_game_over()
 * ---------------
 * Prompt shown when game is over after stdin recieves EOF.
 * 
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void game_prompt_game_over();

/* game_prompt_invalid_game_over_attempts()
 * ---------------
 * Prompt shown when game is over after attempts have run out.
 * 
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void game_prompt_game_over_attempts();

/* game_prompt_invalid_game_win()
 * ---------------
 * Prompt shown when the user wins the game by matching the current word 
 * with target word.
 * 
 * Parameters:
 *  currentSteps: The current number of steps taken in the game. 
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void game_prompt_game_win(unsigned short steps);

#endif
