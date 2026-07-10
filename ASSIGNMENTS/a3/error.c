/*
 * ============================================================================
 *
 *       Filename:  error.c
 *
 *    Description:  Only exit point of program! Handles exit status for all
 *                  errors.
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
#include <stdlib.h>
#include <stdio.h>

#include "error.h"

void error_arguments_invalid() {
    fprintf(stderr, "Usage: testuqwordladder [--diffshow N] "
            "[--testdir dir] [--regen] jobfile programname\n");

    exit(ERR_CMD_ARGS);
}

void error_job_file_invalid(char* fileName) {
    fprintf(stderr, "testuqwordladder: Unable to open job file \"%s\"\n", 
            fileName);

    free(fileName);
    exit(ERR_JOB_FILE);
}

void error_job_file_syntax(char* fileName, unsigned int lineNumber) {
    fprintf(stderr, "testuqwordladder: Syntax error on "
            "line %d of job file \"%s\"\n", lineNumber, fileName);

    free(fileName);
    exit(ERR_JOB_SYNTAX);
}

void error_job_file_duplicate_id(char* fileName, unsigned int lineNumber) {
    fprintf(stderr, "testuqwordladder: Line %d of file \"%s\""
            ": duplicate test ID\n", lineNumber, fileName);

    free(fileName);
    exit(ERR_JOB_ID);
}

void error_job_file_empty(char* fileName) {
    fprintf(stderr, "testuqwordladder: Job spec file \"%s\""
            " is empty\n", fileName);

    free(fileName);
    exit(ERR_JOB_EMPTY);
}

void error_job_file_input(
        char* inputFileName, 
        char* fileName,
        unsigned int lineNumber) {
    fprintf(stderr, "testuqwordladder: Unable to open input file \"%s\""
            " specified on line %d of job file \"%s\"\n", 
            inputFileName, lineNumber, fileName);

    free(inputFileName);
    free(fileName);
    exit(ERR_JOB_INPUT_FILE);
}

void error_test_directory_invalid(char* directoryName) {
    fprintf(stderr, "testuqwordladder: Unable to create directory"
            " named \"%s\"\n", directoryName);

    free(directoryName);
    exit(ERR_TEST_DIRECTORY);
}

void error_test_output_file_invalid(char* pathName) {
    fprintf(stderr, "testuqwordladder: Can't open output file \"%s\""
            " for writing\n", pathName);

    free(pathName);
    exit(ERR_TEST_FILE);
}

void error_test_exit(int exitStatus) {
    exit(exitStatus);
}
