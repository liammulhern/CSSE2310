/*
 * ============================================================================
 *
 *       Filename:  job.h
 *
 *    Description:  Header file for job.c
 *
 *        Version:  1.0
 *        Created:  24/09/23 10:45:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A3 
 *
 * ============================================================================
 */
#ifndef JOB_H
#define JOB_H

#define JOB_TEST_SIZE 1201
#define JOB_MIN_TAB 1

#include <stdlib.h>
#include <stdio.h>

#include "hashtable.h"
#include "list.h"
#include "arguments.h"

/* JobTest 
 * ---------------
 * Struct representing an individual test job read from the job file.
 * 
 * Parameters:
 *  testId: Unique identifier for test job.
 *  testInputFileName: Name of input file that is passed to stdin.
 *  testInputArguments: The arguments that are passed to the tested program
 *      on start.
 *  testInputArgumentsSize: The length of testInputArguments array.
 *  testStdoutPath: The path to the stdout file for the test.
 *  testStderrPath: The path to the stderr file for the test.
 *  testExitStatusPath: The path to the exit status file for the test.
 */
typedef struct JobTest {
    char* testId;
    char* testInputFileName;
    char** testInputArguments;
    int testInputArgumentsSize;
    char* testStdoutPath;
    char* testStderrPath;
    char* testExitStatusPath;
} JobTest;

/* job_get_tests()
 * ---------------
 * Get and parse all jobs from the job file returning hash table of 
 * all valid jobTests.
 * 
 * Parameters:
 *  arguments: The command line arguments given to the program.
 *
 * Returns:
 *  Pointer to hash table containing all valid jobTests.
 * 
 * Errors:
 *  Exits if job file is empty or invalid.
 */
HashTable* job_get_tests(Arguments* arguments);

/* job_read_tests()
 * ---------------
 * Reads job file and creates JobTest structs to add to jobTests hash table.
 * 
 * Parameters:
 *  jobFile: The file to read jobs from.
 *  jobTests: Pointer to hash table containing all valid jobTests.
 *  arguments: The command line arguments given to the program.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits if syntax error, input file error or duplicate ID error.
 */
void job_read_tests(FILE* jobFile, HashTable* jobTests, Arguments* arguments);

/* job_create_test_from_buffer()
 * ---------------
 * Create JobTest struct from job file line by splitting at tab (\t)
 * characters.
 * 
 * Parameters:
 *  jobBuffer: The raw file line buffer from job file.
 *  arguments: The command line arguments given to the program.
 *
 * Returns:
 *  JobTest struct with information on job file line test. 
 * 
 * Errors:
 *  None
 */
JobTest* job_create_test_from_buffer(char* jobBuffer, Arguments* arguments);

/* job_create_test()
 * ---------------
 * Allocate and create JobTest struct populated with parameters.
 * 
 * Parameters:
 *  testId: Unique identifier for test job.
 *  testInputFileName: Name of input file that is passed to stdin.
 *  testInputArguments: The arguments that are passed to the tested program
 *      on start.
 *  testInputArgumentsSize: The length of testInputArguments array.
 *  testOutputDirectory: The directory to output test files to.
 *
 * Returns:
 *  JobTest struct populate with parameters.
 * 
 * Errors:
 *  None
 */
JobTest* job_create_test(
        char* testId,
        char* testInputFileName,
        char** testInputArguments,
        int testInputArgumentsSize,
        char* testOutputDirectory);

/* job_create_output_file_path()
 * ---------------
 * Allocate and create output file path by combining 
 * <directoryName>/<fileName>.<fileExtension>
 * 
 * Parameters:
 *  fileName: The name of the file.
 *  directoryName: The path of the directory.
 *  fileExtension: The file extension.
 *
 * Returns:
 *  Allocated string forming file ouput path.
 * 
 * Errors:
 *  None
 */
char* job_create_output_file_path(
        char* fileName,
        char* directoryName, 
        char* fileExtension);

/* job_destroy_test()
 * ---------------
 * Destroy JobTest struct and free all allocated memory.
 * 
 * Parameters:
 *  object: Generic pointer to JobTest struct to destroy.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void job_destroy_test(void* object);

/* job_print_test()
 * ---------------
 * Print JobTest to stdout.
 * 
 * Parameters:
 *  object: Generic pointer to JobTest struct to print.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void job_print_test(void* object);

/* job_error()
 * ---------------
 * Destroy all allocated memory when error occures in job file.
 * 
 * Parameters:
 *  arguments: The command line arguments given to the program.
 *  jobTests: Pointer to hash table containing all valid jobTests.
 *  jobFile: The file to read jobs from.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
char* job_error(
        Arguments* arguments, 
        HashTable* jobTests, 
        FILE* jobFile);

#endif
