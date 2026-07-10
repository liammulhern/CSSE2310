/*
 * ============================================================================
 *
 *       Filename:  error.h
 *
 *    Description:  Header file for error.c
 *
 *        Version:  1.0
 *        Created:  17/09/23 16:41:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A3
 *
 * ============================================================================
 */
#ifndef ERROR_H
#define ERROR_H

#define ERR_CMD_ARGS 18
#define ERR_JOB_FILE 15
#define ERR_JOB_SYNTAX 17
#define ERR_JOB_ID 16
#define ERR_JOB_INPUT_FILE 5
#define ERR_JOB_EMPTY 11
#define ERR_TEST_DIRECTORY 13
#define ERR_TEST_FILE 7
#define ERR_TEST_ALL_PASS 0
#define ERR_TEST_FAIL 12
#define ERR_TEST_INTERUPTED 6

/* error_arguments_invalid()
 * ---------------
 * Prints usage error to stderr and exits with status ERR_CMD_ARGS.
 * 
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_CMD_ARGS.
 */
void error_arguments_invalid();

/* error_job_file_invalid()
 * ---------------
 * Prints invalid job file prompt to stderr and exits 
 * with status ERR_JOB_FILE.
 * 
 * Parameters:
 *  fileName: The name of the invalid job file.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_JOB_FILE.
 */
void error_job_file_invalid(char* fileName);

/* error_job_file_syntax()
 * ---------------
 * Prints invalid job file syntax prompt to stderr and 
 * exits with status ERR_JOB_SYNTAX.
 * 
 * Parameters:
 *  fileName: The name of the invalid job file.
 *  lineNumber: The line number where the syntax error occured.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_JOB_SYNTAX.
 */
void error_job_file_syntax(char* fileName, unsigned int lineNumber);

/* error_job_file_duplicate_id()
 * ---------------
 * Prints job file duplicate error prompt to stderr and 
 * exits with status ERR_JOB_ID.
 * 
 * Parameters:
 *  fileName: The name of the invalid job file.
 *  lineNumber: The line number where the syntax error occured.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_JOB_ID.
 */
void error_job_file_duplicate_id(char* fileName, unsigned int lineNumber);

/* error_job_file_empty()
 * ---------------
 * Prints job file empty error prompt to stderr and 
 * exits with status ERR_JOB_EMPTY.
 * 
 * Parameters:
 *  fileName: The name of the invalid job file.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_JOB_EMPTY.
 */
void error_job_file_empty(char* fileName);

/* error_job_file_input()
 * ---------------
 * Prints job file invalid input file prompt to stderr and 
 * exits with status ERR_JOB_INPUT_FILE.
 * 
 * Parameters:
 *  inputFileName: The name of the invalid input file.
 *  fileName: The name of the invalid job file.
 *  lineNumber: The line number where the invalid job input file error occured.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_JOB_INPUT_FILE.
 */
void error_job_file_input(
        char* inputFileName, 
        char* fileName,
        unsigned int lineNumber);

/* error_test_directory_invalid()
 * ---------------
 * Prints invalid test directory prompt to stderr and 
 * exits with status ERR_TEST_DIRECTORY.
 * 
 * Parameters:
 *  directoryName: The name of the invalid directory.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_TEST_DIRECTORY.
 */
void error_test_directory_invalid(char* directoryName);

/* error_test_output_file_invalid()
 * ---------------
 * Prints invalid test output file to stderr and 
 * exits with status ERR_TEST_FILE.
 * 
 * Parameters:
 *  pathName: The name of the invalid path.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_TEST_FILE.
 */
void error_test_output_file_invalid(char* pathName);

/* error_test_exit()
 * ---------------
 * Exits with status provided by test output.
 * 
 * Parameters:
 *  exitStatus: The status to exit with.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status exitStatus.
 */
void error_test_exit(int exitStatus);

#endif
