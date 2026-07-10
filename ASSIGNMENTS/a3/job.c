/*
 * ============================================================================
 *
 *       Filename:  job.c
 *
 *    Description:  Creates jobTests from job file after parsing and 
 *                  validating.
 *
 *        Version:  1.0
 *        Created:  17/09/23 21:54:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A3
 *
 * ============================================================================
 */
#include <stdio.h>
#include <csse2310a3.h>
#include <string.h>

#include "job.h"
#include "validity.h"
#include "error.h"

HashTable* job_get_tests(Arguments* arguments) {
    FILE* jobFile = fopen(arguments->testJobFilePath, "r");

    // Check if file path is valid
    if (validity_check_job_file(jobFile)) {
        char* fileName = job_error(arguments, NULL, jobFile);
        error_job_file_invalid(fileName);
    }

    // Create HashTable of job tests
    HashTable* jobTests = hash_table_create(JOB_TEST_SIZE, hash_function, 
            hash_table_compare_char, destroy_null, print_string, 
            job_destroy_test, job_print_test);


    // Read the job file and add all valid jobs to HashTable
    job_read_tests(jobFile, jobTests, arguments);

    // Check if there are valid jobs 
    if (hash_table_empty(jobTests)) {
        char* fileName = job_error(arguments, jobTests, jobFile);
        error_job_file_empty(fileName);
    }

    fclose(jobFile);

    return jobTests;
}

void job_read_tests(
        FILE* jobFile,
        HashTable* jobTests,
        Arguments* arguments) {
    int jobLineNumber = 0;
    char* jobBuffer;

    // Read until EOF
    while ((jobBuffer = read_line(jobFile)) != NULL) {
        jobLineNumber++;

        // Check if line is a valid job
        if (!validity_check_job(jobBuffer)) {
            free(jobBuffer);
            continue;
        }

        JobTest* jobTest = job_create_test_from_buffer(jobBuffer, arguments);
        free(jobBuffer);

        // Check if job had valid syntax
        if (jobTest == NULL) {
            char* fileName = job_error(arguments, jobTests, jobFile);
            error_job_file_syntax(fileName, jobLineNumber);
        }

        // Check if input file is valid 
        if (validity_check_job_input(jobTest->testInputFileName)) {
            char* fileName = job_error(arguments, jobTests, jobFile);
            char* inputFileName = strdup(jobTest->testInputFileName);
            job_destroy_test(jobTest);
            error_job_file_input(inputFileName, fileName, jobLineNumber);
        }
        
        // Check for test id duplicate 
        if (!hash_table_insert(jobTests, jobTest->testId, 
                strlen(jobTest->testId), jobTest)) {
            char* fileName = job_error(arguments, jobTests, jobFile);
            job_destroy_test(jobTest);
            error_job_file_duplicate_id(fileName, jobLineNumber);
        }    
    }

}

JobTest* job_create_test_from_buffer(char* jobBuffer, Arguments* arguments) {
    char** jobBufferSplit = split_string(jobBuffer, '\t');

    // Get the length of the split buffer array
    int length = 0;
    while (jobBufferSplit[length] != NULL) {
        length++;
    }

    int jobSyntaxError = validity_check_job_syntax(jobBufferSplit, length);

    // Check if the job has valid syntax
    if (jobSyntaxError) {
        free(jobBufferSplit);
        return NULL;
    }

    char* testId = strdup(jobBufferSplit[0]);
    char* testInputFileName = strdup(jobBufferSplit[1]);

    // Create a deep copy of the input argument strings and allocate room 
    // for inital null that will become exec program name and final 
    // null terminator for end of arguments.
    char** testInputArguments = (char**) malloc(sizeof(char*) * 
            (length));

    testInputArguments[0] = NULL;

    // Populate allocated memory with copy of split buffer contents
    for (int i = 0; i < length - 2; i++) {
        testInputArguments[i + 1] = strdup(jobBufferSplit[i + 2]);
    }

    testInputArguments[length - 1] = NULL;

    JobTest* jobTest = job_create_test(testId, testInputFileName, 
            testInputArguments, length, arguments->testOutputDirectory);

    free(jobBufferSplit);
    return jobTest;
}

JobTest* job_create_test(
        char* testId, 
        char* testInputFileName, 
        char** testInputArguments,
        int testInputArgumentsSize,
        char* testOutputDirectory) {

    JobTest* jobTest = (JobTest*) malloc(sizeof(JobTest));

    jobTest->testId = testId;
    jobTest->testInputFileName = testInputFileName;
    jobTest->testInputArguments = testInputArguments;
    jobTest->testInputArgumentsSize = testInputArgumentsSize;

    jobTest->testStdoutPath = job_create_output_file_path(testId, 
            testOutputDirectory, "stdout");
    jobTest->testStderrPath = job_create_output_file_path(testId, 
            testOutputDirectory, "stderr");
    jobTest->testExitStatusPath = job_create_output_file_path(testId, 
            testOutputDirectory, "exitstatus");

    return jobTest;
}

char* job_create_output_file_path(
        char* fileName,
        char* directoryName, 
        char* fileExtension) {

    size_t filePathLength = 0;

    filePathLength += strlen(fileName);
    filePathLength += strlen(fileExtension);
    filePathLength += strlen(directoryName);

    // Allocate memory for length of 
    // directoryName + '/' + fileName + '.' + fileExtension + '\0'
    char* filePathBuffer = (char*) malloc(sizeof(char) * (filePathLength + 3));

    snprintf(filePathBuffer, filePathLength + 3, "%s/%s.%s", 
            directoryName, fileName, fileExtension);

    return filePathBuffer;
}

void job_destroy_test(void* object) {
    JobTest* jobTest = (JobTest*) object;

    free(jobTest->testId);
    free(jobTest->testInputFileName);

    for (int i = 0; i < jobTest->testInputArgumentsSize - 1; i++) {
        free(jobTest->testInputArguments[i]);
    }

    free(jobTest->testInputArguments);
    free(jobTest->testStdoutPath);
    free(jobTest->testStderrPath);
    free(jobTest->testExitStatusPath);

    free(jobTest);
}

void job_print_test(void* object) {
    JobTest* jobTest = (JobTest*) object;

    printf("Test ID: %s, Test Input File Name: %s, Test Arguments: [%d] ", 
            jobTest->testId, jobTest->testInputFileName, 
            jobTest->testInputArgumentsSize);

    // Print test input arguments to stdout
    for (int i = 0; i < jobTest->testInputArgumentsSize; i++) {
        printf("%s, ", jobTest->testInputArguments[i]);
    }
}

char* job_error(
        Arguments* arguments, 
        HashTable* jobTests, 
        FILE* jobFile) {

    char* fileName = strdup(arguments->testJobFilePath); 

    if (arguments != NULL) {
        arguments_destroy(arguments);
    }

    if (jobTests != NULL) {
        hash_table_destroy(jobTests);
    }

    if (jobFile != NULL) {
        fclose(jobFile);
    }

    return fileName;
}
