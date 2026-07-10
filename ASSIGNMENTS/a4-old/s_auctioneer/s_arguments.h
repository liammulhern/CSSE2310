/*
 * ============================================================================
 *
 *       Filename:  arguments.h
 *
 *    Description:  Header file for arguments.c
 *
 *        Version:  1.0
 *        Created:  22/10/23 11:50:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4 
 *
 * ============================================================================
 */
#ifndef S_ARGUMENTS_H
#define S_ARGUMENTS_H

#define CMD_ERROR '?'
#define CMD_PORT 'p'
#define CMD_CONNECTION 'c'

#define MIN_PORT 1024
#define MAX_PORT 65535

#include <stdbool.h>

/* Arguments
 * ---------------
 *  Struct representing the arguments entered by the user
 *  from the command line.
 * 
 * Parameters:
 *  portNumber: The port number or service ID to connect to the 
 *      auctioneer server.
 */
typedef struct Arguments {
    char* portNumber;
    size_t maxConnections;
} Arguments;

/* arguments_create()
 * ---------------
 * Initialises Arguments struct values to NULL and returns pointer.
 * 
 * Returns:
 *  Pointer to the initialised Arguments struct.
 * 
 * Errors:
 *  None
 */
Arguments* arguments_create();

/* arguments_get()
 * ---------------
 * Reads the command line arguments given to the terminal using getopt_long
 * command and validates them.
 * 
 * Parameters:
 *  argc: The argument count passed to the main function from the console.
 *  argv: The argument array passed to the main function from the console.
 *
 * Returns:
 *  Pointer to Arguments struct that contains valid command
 *  line arguments and default values if not overwritten.
 * 
 * Errors:
 *  None
 */
Arguments* arguments_get(int argc, char* argv[]);

/* arguments_check_validity()
 * ---------------
 * Parse command line arguments and check if they meet the usage
 * specfication. 
 *
 * Parameters:
 *  arguments: Pointer to Arguments struct that contains command
 *  line arguments and default values if not overwritten.
 *  commandIndex: The index of the command argument in argv.
 *  commandType: The value returned by the getopt_long() representing
 *  the command line argument parsed.
 *  commandArgument: The string following the respective command.
 *  commandCount: The total number of commands present on the command line.
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

char* arguments_check_validity_port(
        Arguments* arguments, 
        char* commandArgument);

size_t arguments_check_validity_max_connection(
        Arguments* arguments,
        char* commandArgument);

bool argument_check_repeated_command_string(char* argument);

bool argument_check_repeated_command_int(int argument);

long arguments_check_valid_number(char* argument);

void arguments_set_default(Arguments* arguments);

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
void arguments_destroy(void* object);

#endif
