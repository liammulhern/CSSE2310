/*
 * ============================================================================
 *
 *       Filename:  validity.h
 *
 *    Description:  Header file for validity.c
 *
 *        Version:  1.0
 *        Created:  24/09/23 13:22:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (), 
 *   Organization:  
 *
 * ============================================================================
 */
#ifndef VALIDITY_H
#define VALIDITY_H

#define MIN_JOB_ELEMENTS 2

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/* validitiy_check_job()
 * ---------------
 * Check if a job buffer contains no characters or a comment.
 * 
 * Parameters:
 *  jobBuffer: The job file line to check.
 *
 * Returns:
 *  True if job buffer does not contain comment or no characters.
 * 
 * Errors:
 *  None
 */
bool validity_check_job(char* jobBuffer);

/* validitiy_check_job_syntax()
 * ---------------
 * Check split job buffer for valid syntax.
 * 
 * Parameters:
 *  jobBufferSplit: Array of job buffer elements split at tabs.
 *  length: The number of elements in jobBufferSplit.
 *
 * Returns:
 *  Exit status integer if syntax error occurs, 0 otherwise.
 * 
 * Errors:
 *  None
 */
int validity_check_job_syntax(char** jobBufferSplit, size_t length);

/* validitiy_check_contains_tabs()
 * ---------------
 * Check that the job buffer contains at least the minimum number of 
 * job elements.
 * 
 * Parameters:
 *  length: The number of elements in jobBufferSplit.
 *
 * Returns:
 *  True if job buffer does not contain the minimum number of tabs,
 *  false otherwise.
 * 
 * Errors:
 *  None
 */
bool validity_check_contains_tab(size_t length);

/* validitiy_check_leading_characters()
 * ---------------
 * Check that the job buffer element is not empty.
 * 
 * Parameters:
 *  jobBufferSplit: Array of job buffer elements split at tabs.
 *  index: The index of jobBufferSplit to check for characters.
 *
 * Returns:
 *  True if the index contains no characters, false otherwise.
 * 
 * Errors:
 *  None
 */
bool validity_check_leading_characters(char** jobBufferSplit, size_t index);

/* validitiy_check_test_id_syntax()
 * ---------------
 * Check that test id does not contain '/' character.
 * 
 * Parameters:
 *  jobBufferSplit: Array of job buffer elements split at tabs.
 *
 * Returns:
 *  True if jobBufferSplit contains '/' character, false otherwise.
 * 
 * Errors:
 *  None
 */
bool validity_check_test_id_syntax(char** jobBufferSplit);

/* validitiy_check_job_file()
 * ---------------
 * Check if job file can be opened.
 * 
 * Parameters:
 *  jobFile: Job file that has been opened.
 *
 * Returns:
 *  Exit status if job file could not be opened, 0 otherwise.
 * 
 * Errors:
 *  None
 */
int validity_check_job_file(FILE* jobFile);

/* validitiy_check_job_input()
 * ---------------
 * Check if job input file can be opened.
 * 
 * Parameters:
 *  inputFilePath: The file path of the input file.
 *
 * Returns:
 *  Exit status if input file cannot be opened, 0 otherwise.
 * 
 * Errors:
 *  None
 */
int validity_check_job_input(char* inputFilePath);

#endif
