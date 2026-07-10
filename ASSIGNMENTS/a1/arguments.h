/*
 * ============================================================================
 *
 *       fileName:  arguments.h
 *
 *    Description:  Header file for arguments.c
 *
 *        Version:  1.0
 *        Created:  04/08/23 21:33:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern 
 *   Organization:  CSSE2310 A1
 *
 * ============================================================================
 */
#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#define START_WORD 's'
#define TARGET_WORD 't'
#define WORD_LENGTH 'l'
#define STEP_LIMIT 'i'
#define DICT_FILE 'd'
#define CMD_ERROR '?'

#include <stdbool.h>

/* Arguments
 * ---------------
 * Struct representing the arguments entered by the user
 * from the command line.
 * 
 * Parameters:
 *  sourceWord: The game's starting word.
 *  finalWord: The game's target word.
 *  wordLength: The number of characters in the word.
 *  stepLimit: The maximum number of steps that the game can be completed in.
 *  fileName: The name of the file containing the dictionary words.
 */
typedef struct Arguments {
    char* sourceWord;
    char* finalWord;
    int wordLength;
    int stepLimit;
    char* fileName;
} Arguments;

/* arguments_create()
 * ---------------
 * Initialises Arguments struct values to NULL and returns pointer.
 *
 * Returns:
 *  Pointer to initialised Arguments struct.
 * 
 * Errors:
 *  None
 */
Arguments* arguments_create();

/* arguments_get() 
 * ---------------
 * Reads the command line arguments given in the console using getopt_long
 * command and validates them.
 *
 * Parameters:
 *  argc: The argument count passed to the main function from the console.
 *  argv: The argument array passed to the main function from the console.
 *
 * Returns:
 *  Pointer to Arguments struct that contains valid command
 *	line arguments and default values if not overwritten.
 *
 * Errors:
 *  Exits via arguments_invalid() if command line arguments are invalid.
 *
 * REF: This code is inspired by the code given in getopt (3) man page. It 
 * REF: has been modified to fit the needs of the function. 
 */
Arguments* arguments_get(int argc, char* argv[]);

/* arguments_check_validity()
 * ---------------
 * Parse command line arguments and check if they meet the usage
 * specfication. 
 *
 * Parameters:
 *  arguments: Pointer to Arguments struct that contains command
 *	line arguments and default values if not overwritten.
 *  commandType: The value returned by the getopt_long() representing
 *	the command line argument parsed.
 *  commandArgument: The string following the respective command.
 *
 * Returns:
 *  None
 *
 * Errors:
 *  Exits via error_arguments_invalid() if command line arguments are invalid.
 */
void arguments_check_validity(
	Arguments* arguments, 
	int commandType, 
	char* commandArgument);

/* arguments_check_validity_char()
 * ---------------
 * Check if a string command line argument is valid.
 * 
 * Parameters:
 *  arguments: The Arguments struct to destroy if invalid.
 *  argument: The argument currently stored in the arguments struct.
 *  commandArgument: The command line argument to test validity.
 *
 * Returns:
 *  Valid string.
 * 
 * Errors:
 *  None
 */
char* arguments_check_validity_char(
	Arguments* arguments, 
	char* argument,
	char* commandArgument);

/* arguments_check_validity_int()
 * ---------------
 * Check if an integer command line argument is valid. 
 * 
 * Parameters:
 *  arguments: The Arguments struct to destroy if invalid.
 *  argument: The argument currently stored in the arguments struct.
 *  commandArgument: The command line argument to test validity.
 *
 * Returns:
 *  The valid integer.
 * 
 * Errors:
 *  None
 */
int arguments_check_validity_int(
	Arguments* arguments, 
	int argument,
	char* commandArgument);

/* arguments_check_value_validity()
 * ---------------
 * Checks if a valid option argument is followed by an associated 
 * value argument. If invalid returns true. 
 *
 * Parameters:
 *  commandArgument: The string following the respective command.
 *
 * Returns:
 *  True if value is invalid, false otherwise.
 *
 * Errors:
 *  None
 */
bool arguments_check_value_validity(char* commandArgument);

/* arguments_check_repeated_command_char()
 * ---------------
 * Check if command is repeated for string type arguments by determining
 * if the default value is overwritten.
 * 
 * Parameters:
 *  argument: String from Arguments struct.
 *
 * Returns:
 *  True if value is invalid, false otherwise.
 * 
 * Errors:
 *  None
 */
bool arguments_check_repeated_command_char(char* argument);

/* arguments_check_repeated_command_int()
 * ---------------
 * Check if command is repeated for int type arguments by determining
 * if the default value is overwritten.
 * 
 * Parameters:
 *  argument: int type from Arguments struct.
 *
 * Returns:
 *  True if value is invalid, false otherwise.
 * 
 * Errors:
 *  None
 */
bool arguments_check_repeated_command_int(int argument);

/* arguments_check_int_validity()
 * ---------------
 * Checks if a valid option argument is a valid positive integer. If valid 
 * returns positive integer, otherwise calls arguments_invalid().
 *
 * Parameters:
 *  commandArgument: The string following the respective command.
 *
 * Returns:
 *  If valid returns positive integer, otherwise raises error 
 *  via arguments_invalid().
 *
 * Errors:
 *  None
 */
int arguments_check_int_validity(char* commandArgument);

/* arguments_print()
 * ---------------
 * Prints Arguments struct to the terminal.
 * 
 * Parameters:
 *  arguments: The arguments to print.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void arguments_print(Arguments* arguments);

/* arguments_destroy()
 * ---------------
 * Destroys argument struct and frees allocated memory.
 * 
 * Parameters:
 *  arguments: The argument struct to destroy.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void arguments_destroy(Arguments* arguments);

#endif
