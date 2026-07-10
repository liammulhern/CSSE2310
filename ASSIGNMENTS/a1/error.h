/*
 * ============================================================================
 *
 *       Filename:  error.h
 *
 *    Description:  Header file for error.c
 *
 *        Version:  1.0
 *        Created:  16/08/23 16:18:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern
 *   Organization:  CSSE2310 A1 
 *
 * ============================================================================
 */
#ifndef ERROR_H
#define ERROR_H

#define ERR_CMD_ARGS 3
#define ERR_LEN_CONFLICT 15
#define ERR_LEN_RANGE 2
#define ERR_WORD_CHARS 9
#define ERR_WORD_DIFF 5
#define ERR_LIMIT_RANGE 18
#define ERR_DICT_FILE 12
#define ERR_WIN 0
#define ERR_GAME_OVER 4
#define ERR_EOF 6

#include "arguments.h"

/* error_exit()
 * ---------------
 * Exit program depending on status given after printing prompt to stderr.
 * 
 * Parameters:
 *  status: The status to exit with.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits program with status error.
 */
void error_exit(int status);

/* error_arguments_invalid()
 * ---------------
 * Error is present in command line arguments, prints the 
 * following message to standard error and exits with status of 
 * ERR_CMD_ARGS (3).
 * 
 * "Usage: uqwordladder [--start sourceWord] [--target finalWord] 
 * [--limit stepLimit] [--len wordLen] [--dictfile fileName]" 
 *
 * Returns:
 *  None
 *
 * Errors:
 *  Exits program with status ERR_CMD_ARGS (3) if command line 
 *  arguments are invalid.
 */
void error_arguments_invalid();

/* error_word_length_range()
 * ---------------
 * Arguments contain word length outside of the bound 2 and 9 inclusive. 
 * Prints the following to stderr and exits with status ERR_LEN_RANGE (2).
 *
 * "uqwordladder: Word length should be from 2 to 9 (inclusive)"
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_LEN_RANGE (2).
 */
void error_word_length_range();

/* error_word_length_conflict()
 * ---------------
 * Arguments contains word lengths that are inconsistent. Prints the following
 * to stderr and exits with status ERR_LEN_CONFLICT (15).
 *
 * "uqwordladder: Word length conflict - lengths should be consistent"
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_LEN_CONFLICT (15).
 */
void error_word_length_conflict();

/* error_word_difference()
 * ---------------
 * Arguments contain words that are not different. Prints the following to
 * stderr and exits with status ERR_WORD_DIFF (5).
 *
 * "uqwordladder: Words must be different"
 * 
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_WORD_DIFF (5).
 */
void error_word_difference();

/* error_word_contains_invalid_char()
 * ---------------
 * Argument contains characters other thab letters. Prints the following to 
 * stderr and exits with status ERR_WORD_CHARS (9).
 *
 * "uqwordladder: Words must not contain non-letters"
 * 
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_WORD_CHARS (9).
 */
void error_word_contains_invalid_char();

/* error_step_limit_range()
 * ---------------
 * Argument step limit exceeds range. Prints the following to stderr and 
 * exits with status ERR_LIMIT_RANGE (18).
 *
 * "uqwordladder: Step limit must be word length to 50 (inclusive)"
 * 
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_LIMIT_RANGE (18).
 */
void error_step_limit_range();

/* error_dictionary_file_invalid()
 * ---------------
 * Prints the following to stderr and exits with status ERR_DICT_FILE (12).
 *
 * "uqwordladder: File "fileName " cannot be opened"
 * 
 * Parameters:
 *  fileName: The file name that could not be opened.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_DICT_FILE (12)
 */
void error_dictionary_file_invalid(char* fileName);

/* error_end_of_file()
 * ---------------
 * If EOF is read from stdin exits with status ERR_EOF (6).
 * 
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_EOF (6)
 */
void error_end_of_file();

/* error_win()
 * ---------------
 * If game finishes successfully exits with status ERR_WIN (0).
 * 
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_WIN (0).
 */
void error_win();

/* error_game_over()
 * ---------------
 * If game finishes as game over state exits with status ERR_GAME_OVER (4).
 * 
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_GAME_OVER (4).
 */
void error_game_over();

#endif
