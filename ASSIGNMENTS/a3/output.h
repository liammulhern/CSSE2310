/*
 * ============================================================================
 *
 *       Filename:  output.h
 *
 *    Description:  Header file for output.c
 *
 *        Version:  1.0
 *        Created:  29/09/23 22:25:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A3
 *
 * ============================================================================
 */
#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdbool.h>

#include "arguments.h"
#include "hashtable.h"
#include "job.h"

/* output_create_directory()
 * ---------------
 * Attempts to create the test output directory and if 
 * it fails exits.
 * 
 * Parameters:
 *  arguments: The command line arguments given to the program.
 *  jobTests: The hash table of job tests to complete.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits if directory cannot be opened or created.
 */
void output_create_directory(Arguments* arguments, HashTable* jobTests);

/* output_create_files()
 * ---------------
 * Iterate over all JobTest structs and create required file for each.
 *
 * Parameters:
 *  arguments: The command line arguments given to the program.
 *  jobTests: The hash table of job tests to complete.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void output_create_files(Arguments* arguments, HashTable* jobTests);

/* output_create_test_files()
 * ---------------
 * Create stdout, stderr, and exit status output files for JobTest.
 * 
 * Parameters:
 *  arguments: The command line arguments given to the program.
 *  jobTests: The hash table of job tests to complete.
 *  jobTest: The test to create output files for.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void output_create_test_files(
        Arguments* arguments,
        HashTable* jobTests,
        JobTest* jobTest);

/* output_check_regenerate_test_file()
 * ---------------
 * Check if a file needs to be (re)generated 
 * i.e. new, modified, or flagged
 * 
 * Parameters:
 *  arguments: The command line arguments given to the program.
 *  jobTests: The hash table of job tests to complete.
 *  filePath: The file path to compare modified time to.
 *
 * Returns:
 *  True if file is (re)generated, false otherwise.
 * 
 * Errors:
 *  None
 */
bool output_check_regenerate_test_file(
        Arguments* arguments,
        HashTable* jobTests,
        char* filePath);

/* output_create_test_file()
 * ---------------
 * Create test file at file path.
 * 
 * Parameters:
 *  arguments: The command line arguments given to the program.
 *  jobTests: The hash table of job tests to complete.
 *  filePath: The file path to create new file at.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exit if file path could not be opened for writing.
 */
void output_create_test_file(
        Arguments* arguments,
        HashTable* jobTests,
        char* filePath);

/* output_generate_expected_test()
 * ---------------
 * Generate expected output of test program 
 * to files for given jobTest.
 * 
 * Parameters:
 *  jobTest: The job test to generate expected output for.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void output_generate_expected_test(JobTest* jobTest);

/* output_check_directory()
 * ---------------
 * Attempt to create directory and return true if failed.
 * 
 * Parameters:
 *  directoryName: The path of the desired directory.
 *
 * Returns:
 *  True if directory could not be made, false otherwise.
 * 
 * Errors:
 *  None
 */
bool output_check_directory(char* directoryName);

/* output_check_file_modification()
 * ---------------
 * Check if a job file has been modifed after the output 
 * file has been generated.
 * 
 * Parameters:
 *  jobFilePath: The job file path.
 *  outputFilePath: The output file path.
 *
 * Returns:
 *  True if files have been modified, false otherwise.
 * 
 * Errors:
 *  None
 */
bool output_check_file_modification(
        char* jobFilePath, 
        char* outputFilePath);

/* output_get_file_modification_time()
 * ---------------
 * Get the modification time of a file.
 * 
 * Parameters:
 *  filePath: The file path to check modification time.
 *  filePathError: Flag set if modification time of file 
 *      could not be determined.
 *
 * Returns:
 *  Timespec struct representing modification time.
 * 
 * Errors:
 *  None
 */
struct timespec output_get_file_modification_time(char* filePath, 
        int* filePathError);

#endif
