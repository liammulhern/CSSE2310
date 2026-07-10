/*
 * ============================================================================
 *
 *       Filename:  error.c
 *
 *    Description:  Only exit point of program! Handles exit status for all
 *		    errors.
 *
 *        Version:  1.0
 *        Created:  16/08/23 16:17:54
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

#include "error.h"

void error_exit(int status) {
    switch (status) {
	case ERR_LEN_CONFLICT:
	    error_word_length_conflict();
	    break;
	case ERR_LEN_RANGE:
	    error_word_length_range();
	    break;
	case ERR_WORD_CHARS:
	    error_word_contains_invalid_char();
	    break;
	case ERR_WORD_DIFF:
	    error_word_difference();
	    break;
	case ERR_LIMIT_RANGE:
	    error_step_limit_range();
	    break;
	case ERR_EOF:
	    error_end_of_file();
	    break;
    }
}

void error_arguments_invalid() {
    fprintf(stderr, "Usage: uqwordladder [--start sourceWord] "
	    "[--target finalWord] [--limit stepLimit] [--len wordLen] "
	    "[--dictfile filename]\n");

    exit(ERR_CMD_ARGS);
}

void error_word_length_range() {
    fprintf(stderr, "uqwordladder: Word length should be"
	    " from 2 to 9 (inclusive)\n");	
	
    exit(ERR_LEN_RANGE);
}

void error_word_length_conflict() {
    fprintf(stderr, "uqwordladder: Word length conflict"
	    " - lengths should be consistent\n");

    exit(ERR_LEN_CONFLICT);
}

void error_word_difference() {
    fprintf(stderr, "uqwordladder: Words must be different\n");

    exit(ERR_WORD_DIFF);
}

void error_word_contains_invalid_char() {
    fprintf(stderr, "uqwordladder: Words must not contain"
	    " non-letters\n");

    exit(ERR_WORD_CHARS);
}

void error_step_limit_range() {
    fprintf(stderr, "uqwordladder: Step limit must be word length to 50"
	    " (inclusive)\n");

    exit(ERR_LIMIT_RANGE);
}

void error_dictionary_file_invalid(char* fileName) {
    fprintf(stderr, "uqwordladder: File \"%s\" cannot be opened\n", 
	    fileName);
    
    free(fileName);
    exit(ERR_DICT_FILE);
}

void error_end_of_file() {
    exit(ERR_EOF);
}

void error_win() {
    exit(ERR_WIN);
}

void error_game_over() {
    exit(ERR_GAME_OVER);
}
