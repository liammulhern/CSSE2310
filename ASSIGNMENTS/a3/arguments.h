/*
 * ============================================================================
 *
 *       Filename:  arguments.h
 *
 *    Description:  Header file for arguments.c
 *
 *        Version:  1.0
 *        Created:  17/09/23 11:11:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A3 
 *
 * ============================================================================
 */
#ifndef ARGUMENTS_H
#define ARGUMENTS_H
#include <stdbool.h>

#define TEST_DIR 'd'
#define TEST_REGEN 'r'
#define MANDATORY 1
#define CMD_ERROR '?'
#define MANDATORY_ARG_COUNT 2
#define DEFAULT_TEST_DIR "./tmp"

/* Arguments
 * ---------------
 *  Struct representing the arguments entered by the user
 *  from the command line.
 * 
 * Parameters:
 *  testOutputDirectory: The directory where the expected output files
 *  are to be saved.
 *  testsRegenerate: Flag to regenerate all expected output files if true.
 *  Otherwise, only files that are not present are generated.
 *  testJobFilePath: The path to the file containing the tests to be run.
 *  testProgramPath: The path to the executable the will be tested.
 */
typedef struct Arguments {
    char* testOutputDirectory;
    bool testsRegenerate;
    char* testJobFilePath;
    char* testProgramPath;
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
        int commandIndex,
        int commandType, 
        char* commandArgument,
        int commandCount);

/* arguments_set_mandatory()
 * ---------------
 * Set the mandatory command line arguments.
 * 
 * Parameters:
 *  arguments: Pointer to Arguments struct that contains command
 *  line arguments and default values if not overwritten.
 *  commandIndex: The index of the command argument in argv.
 *  commandArgument: The string following the respective command.
 *  mandatoryStartIndex: The start index of the mandatory command arguments.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void arguments_set_mandatory(
        Arguments* arguments, 
        int commandIndex,
        char* commandArgument,
        int mandatoryStartIndex);

/* arguments_check_validity_mandatory()
 * ---------------
 * Check if all mandatory command line arguments have been entered. If None
 * raise argument error.
 * 
 * Parameters:
 *  arguments: Pointer to Arguments struct that contains command
 *  line arguments and default values if not overwritten.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits via error_arguments_invalid() if command line arguments are invalid.
 */
void arguments_check_validity_mandatory(Arguments* arguments);

/* arguments_check_validity_string()
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
char* arguments_check_validity_string(
        Arguments* arguments,
        char* argument,
        char* commandArgument);

/* arguments_check_repeated_command_string()
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
bool arguments_check_repeated_command_string(char* argument);

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
        int commandArgument);

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

/* arguments_set_default()
 * ---------------
 * Set the default argument fields if they are not already set.
 * 
 * Parameters:
 *  arguments: The arguments to set set defaults.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
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

