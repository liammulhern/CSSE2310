/*
 * ============================================================================
 *
 *       Filename:  test.h
 *
 *    Description:  Header file for test.c
 *
 *        Version:  1.0
 *        Created:  17/09/23 16:18:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874),
 *   Organization:  CSSE2310 A3 
 *
 * ============================================================================
 */
#ifndef TEST_H
#define TEST_H

#include "arguments.h"
#include "hashtable.h"
#include "job.h"
#include "run.h"

/* test_start()
 * ---------------
 * Start running all tests by getting arguments, jobs, and 
 * comparing them to expected output.
 * 
 * Parameters:
 *  argc: The number of command line arguments specified 
 *      on program execution
 *  argv: The command line arguments specified on program execution.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits via error handler or with exitStatus if tests succeed.
 */
void test_start(int argc, char* argv[]);

/* test_get_arguments()
 * ---------------
 * Get the parsed command line arguments specified at program execution.
 * 
 * Parameters:
 *  argc: The number of command line arguments specified 
 *      on program execution
 *  argv: The command line arguments specified on program execution.
 *
 * Returns:
 *  Pointer to Arguments struct containing parsed arguments.
 * 
 * Errors:
 *  Exits if error occured parsing arguments.
 */
Arguments* test_get_arguments(int argc, char* argv[]);

/* test_get_jobs()
 * ---------------
 * Get the parsed job file jobs specified in the command line argument.
 * 
 * Parameters:
 *  arguments: The parsed command line arguments.
 *
 * Returns:
 *  Pointer to HashTable containing valid JobTest structs.
 * 
 * Errors:
 *  Exits if error occured parsing job file.
 */
HashTable* test_get_jobs(Arguments* arguments);

/* test_destroy()
 * ---------------
 * Destroy and free allocated memory associated with running tests.
 * 
 * Parameters:
 *  arguments: The arguments to destroy.
 *  jobTests: The jobTest structs to destroy.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void test_destroy(Arguments* arguments, HashTable* jobTests);

#endif
