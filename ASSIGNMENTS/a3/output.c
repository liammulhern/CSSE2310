/*
 * ============================================================================
 *
 *       Filename:  output.c
 *
 *    Description:  Generate output files and directories for tests.
 *
 *        Version:  1.0
 *        Created:  29/09/23 22:18:06
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
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <csse2310a3.h>

#include "output.h"
#include "run.h"
#include "test.h"
#include "error.h"
#include "validity.h"

void output_create_directory(
        Arguments* arguments, 
        HashTable* jobTests) {

    // Check if file directory failed to be created 
    if (output_check_directory(arguments->testOutputDirectory)
            && errno != EEXIST) {
        char* directoryName = strdup(arguments->testOutputDirectory);
        test_destroy(arguments, jobTests);
        error_test_directory_invalid(directoryName);
    } 
}

void output_create_files(Arguments* arguments, HashTable* jobTests) {
    size_t index = 0;
    HashTableNode* currentNode = NULL; 

    // Iterate over HashTable and create output files
    while ((currentNode = hash_table_iterate(jobTests,
            currentNode, &index)) != NULL) {
        JobTest* jobTest = (JobTest*) currentNode->object;
        output_create_test_files(arguments, jobTests, jobTest);
    }
}

void output_create_test_files(
        Arguments* arguments,
        HashTable* jobTests,
        JobTest* jobTest) {

    // If flag changes from 0 then an output file needs to be created 
    // or updated.
    int outputFileCreated = 0;

    outputFileCreated += output_check_regenerate_test_file(arguments, jobTests,
            jobTest->testStdoutPath);
    outputFileCreated += output_check_regenerate_test_file(arguments, jobTests,
            jobTest->testStderrPath);
    outputFileCreated += output_check_regenerate_test_file(arguments, jobTests,
            jobTest->testExitStatusPath);

    if (outputFileCreated) {
        printf("Building expected output for test %s\n", jobTest->testId);

        output_create_test_file(arguments, jobTests, 
                jobTest->testStdoutPath);
        output_create_test_file(arguments, jobTests, 
                jobTest->testStderrPath);
        output_create_test_file(arguments, jobTests, 
                jobTest->testExitStatusPath);

        output_generate_expected_test(jobTest);
    }
}

bool output_check_regenerate_test_file(
        Arguments* arguments,
        HashTable* jobTests,
        char* filePath) {

    // Skip regenerating files if --regen flag is 0 and the job file 
    // was updated before any exisiting files.
    return arguments->testsRegenerate || output_check_file_modification(
            arguments->testJobFilePath, filePath);
}

void output_create_test_file(
        Arguments* arguments,
        HashTable* jobTests,
        char* filePath) {

    FILE* outputFile = fopen(filePath, "w");

    if (outputFile == NULL) {
        char* filePathName = strdup(filePath);
        test_destroy(arguments, jobTests);
        error_test_output_file_invalid(filePathName);
    }

    fclose(outputFile);
}

void output_generate_expected_test(JobTest* jobTest) {
    pid_t generateForkId = fork();
    
    if (generateForkId == 0) {
        // Child process
        int inputFd = open(jobTest->testInputFileName, 
                O_RDONLY, S_IRUSR);
        int stdoutOutputFd = open(jobTest->testStdoutPath, 
                O_WRONLY, S_IWUSR);
        int stderrOutputFd = open(jobTest->testStderrPath, 
                O_WRONLY, S_IWUSR);

        // Check if files can be opened.
        if (inputFd == -1 || stdoutOutputFd == -1 || stderrOutputFd == -1) {
            exit(RUN_EXEC_FAIL);
        }

        // Change child processes stdin to input file
        dup2(inputFd, STDIN_FILENO);
        // Change child processes stdout to output file
        dup2(stdoutOutputFd, STDOUT_FILENO);
        // Change child processes stderr to output file
        dup2(stderrOutputFd, STDERR_FILENO);

        close(inputFd);
        close(stdoutOutputFd);
        close(stderrOutputFd);

        // Launch expected output program
        jobTest->testInputArguments[0] = RUN_EXPECTED;
        execvp(RUN_EXPECTED, jobTest->testInputArguments);

        exit(RUN_EXEC_FAIL);
    }

    // Parent process
    int waitStatus;
    wait(&waitStatus);
    int generateExitStatus = WEXITSTATUS(waitStatus);

    if (generateExitStatus == RUN_EXEC_FAIL) {
        printf("FAILED FILE OPENING\n");
    }

    FILE* exitOutputFile = fopen(jobTest->testExitStatusPath, "w");
    fprintf(exitOutputFile, "%d\n", generateExitStatus);
    fclose(exitOutputFile);
}

bool output_check_directory(char* directoryName) {
    return mkdir(directoryName, S_IRWXU) == -1;
}

bool output_check_file_modification(
        char* jobFilePath, 
        char* outputFilePath) {

    // If flag changes error occured when checking modification time.
    int filePathError = 0;

    struct timespec jobFileTime = output_get_file_modification_time(
            jobFilePath, &filePathError);
    struct timespec outputFileTime = output_get_file_modification_time(
            outputFilePath, &filePathError);
    
    if (filePathError) {
        return true;
    }

    return compare_timespecs(jobFileTime, outputFileTime) > 0;
}

struct timespec output_get_file_modification_time(char* filePath, 
        int* filePathError) {

    struct stat fileAttributes;
    struct timespec ts;

    *filePathError = stat(filePath, &fileAttributes);

    if (!*filePathError) {
        ts = fileAttributes.st_mtim;
    }

    return ts;
}
