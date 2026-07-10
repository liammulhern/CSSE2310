/*
 * ============================================================================
 *
 *       fileName:  arguments.c
 *
 *    Description:  Gets command line arguments from terminal
 *		    and checks validitity before saving valid arguments
 *		    locally until required.
 *
 *        Version:  1.0
 *        Created:  04/08/23 21:30:31
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
#include <getopt.h>
#include <errno.h>
#include <limits.h>

#include "arguments.h"
#include "error.h"

Arguments* arguments_create() {
    Arguments* arguments = (Arguments*) malloc(sizeof(Arguments));

    arguments->sourceWord = "";
    arguments->finalWord = "";
    arguments->wordLength = 0;
    arguments->stepLimit = 0;
    arguments->fileName = "";

    return arguments;
}

Arguments* arguments_get(int argc, char* argv[]) {
    Arguments* arguments = arguments_create();

    // Disable default error output from getopt_long
    opterr = 0;
    
    while(true) {
	// Create command line arguments 
	static struct option ArgumentCommands[] = {
	    {"start", required_argument, 0, START_WORD},
	    {"target", required_argument, 0, TARGET_WORD},
	    {"len", required_argument, 0, WORD_LENGTH},
	    {"limit", required_argument, 0, STEP_LIMIT},
	    {"dictfile", required_argument, 0, DICT_FILE},
	    {0, 0, 0, 0}
	};
	
        // Iterate over command line arguments using long options 
        int commandType = getopt_long(argc, argv, "", ArgumentCommands, NULL);

        // Check if all command line arguments have been read 
        if (commandType == -1) {
	    // If invalid arguments are given throw error
	    if (argc != optind) {
		arguments_destroy(arguments);
		error_arguments_invalid();
	    }
            break;
        }
	
	// The optarg variable is set by getopt_long and represents 
	// the argument following the command.
        arguments_check_validity(arguments, commandType, optarg);
    }
	
    return arguments;
}

void arguments_check_validity(
	Arguments* arguments, 
	int commandType, 
	char* commandArgument) {

    switch (commandType) {
        case START_WORD:
	    // Set valid source word
	    arguments->sourceWord = arguments_check_validity_char(arguments, 
		    arguments->sourceWord, commandArgument);
            break;
        case TARGET_WORD:
	    // Set valid final word
	    arguments->finalWord = arguments_check_validity_char(arguments, 
		arguments->finalWord, commandArgument);
            break;
        case DICT_FILE:
	    // Set valid dictionary file name
            arguments->fileName = arguments_check_validity_char(arguments, 
		arguments->fileName, commandArgument);
	    break;
	case WORD_LENGTH:
	    // Set valid word length
	    arguments->wordLength = arguments_check_validity_int(arguments, 
		    arguments->wordLength, commandArgument);
            break;
        case STEP_LIMIT:
	    // Set valid step limit
	    arguments->stepLimit = arguments_check_validity_int(arguments, 
		    arguments->stepLimit, commandArgument);
            break;
        case CMD_ERROR:
        default:
	    // If unknown argument is present free memory and exit
	    arguments_destroy(arguments);
	    error_arguments_invalid();
            break;
    }
}

char* arguments_check_validity_char(
	Arguments* arguments, 
	char* argument,
	char* commandArgument) {
    
    // If argumentError flag changes from 0 an error occured when validating
    int argumentError = 0;

    argumentError += arguments_check_repeated_command_char(argument);
    argumentError += arguments_check_value_validity(commandArgument);

    if (argumentError) {
	arguments_destroy(arguments);
	error_arguments_invalid();
    }
    
    return strdup(commandArgument);
}

int arguments_check_validity_int(
	Arguments* arguments, 
	int argument,
	char* commandArgument) {
    
    // If argumentError flag changes from 0 an error occured when validating
    int argumentError = 0;

    argumentError += arguments_check_repeated_command_int(argument);
    argumentError += arguments_check_value_validity(commandArgument);
    int argumentNumber = arguments_check_int_validity(commandArgument);

    if (argumentError || !argumentNumber) {
	arguments_destroy(arguments);
	error_arguments_invalid();
    }

    return argumentNumber; 
}

bool arguments_check_value_validity(char* commandArgument) {
    return strlen(commandArgument) == 0;
}

bool arguments_check_repeated_command_char(char* argument) {
    return argument[0];
}

bool arguments_check_repeated_command_int(int argument) {
    return argument;
}

int arguments_check_int_validity(char* commandArgument) {
    char* numberInvalidFlag = NULL;
    
    // Error number for strtol function
    errno = 0;

    long number = strtol(commandArgument, &numberInvalidFlag, 10);
    
    // Check for errors in strtol function
    if (((number == LONG_MIN || number == LONG_MAX) && errno == ERANGE)
	    || numberInvalidFlag == commandArgument
	    || *numberInvalidFlag
	    || number <= 0) {
	return 0;
    } 

    return number;
}

void arguments_print(Arguments* arguments) {
    printf("Start: %s\n", arguments->sourceWord);
    printf("Final: %s\n", arguments->finalWord);
    printf("Length: %d\n", arguments->wordLength);
    printf("Limit: %d\n", arguments->stepLimit);
    printf("Dict File: %s\n", arguments->fileName);
}

void arguments_destroy(Arguments* arguments) {
    // If arguments exist free memory allocated.
    if (arguments->sourceWord[0]) {
	free(arguments->sourceWord);
    }

    if (arguments->finalWord[0]) {
	free(arguments->finalWord);
    }

    if (arguments->fileName[0]) {
	free(arguments->fileName);
    }

    free(arguments);
}
