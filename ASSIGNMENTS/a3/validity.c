/*
 * ============================================================================
 *
 *       Filename:  validity.c
 *
 *    Description:  Check the validity of job files and command line
 *                  arguments.
 *
 *        Version:  1.0
 *        Created:  24/09/23 13:18:21
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
#include <string.h>

#include "error.h"
#include "validity.h"

bool validity_check_job(char* jobBuffer) {
    return jobBuffer[0] != '#' && strlen(jobBuffer) != 0;
}

int validity_check_job_syntax(char** jobBufferSplit, size_t length) {

    if (validity_check_contains_tab(length)
            || validity_check_leading_characters(jobBufferSplit, 0)
            || validity_check_leading_characters(jobBufferSplit, 1)
            || validity_check_test_id_syntax(jobBufferSplit)) {
        return ERR_JOB_SYNTAX;
    }

    return 0;
}

bool validity_check_contains_tab(size_t length) {
    // Check if line in job file contains a tab character
    return length < MIN_JOB_ELEMENTS;
}

bool validity_check_leading_characters(char** jobBufferSplit, size_t index) {
    // Check if string at index contains characters. 
    return strlen(jobBufferSplit[index]) == 0;
}

bool validity_check_test_id_syntax(char** jobBufferSplit) {
    return strchr(jobBufferSplit[0], '/') != NULL;
}

int validity_check_job_file(FILE* jobFile) {
    if (jobFile == NULL) {
        return ERR_JOB_FILE;
    }

    return 0;
}

int validity_check_job_input(char* inputFilePath) {
    FILE* testInputFile = fopen(inputFilePath, "r");

    if (testInputFile == NULL) {
        return ERR_JOB_INPUT_FILE;
    }

    fclose(testInputFile);
    return 0;
}
