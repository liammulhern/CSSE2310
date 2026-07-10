/*
 * ============================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  Runs all test cases for a given job file.
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

#include "test.h"
#include "output.h"
#include "error.h"

void test_start(int argc, char* argv[]) {
    run_create_child_signal();
    run_create_interupt_singal();

    Arguments* arguments = test_get_arguments(argc, argv);
    HashTable* jobTests = test_get_jobs(arguments);

    output_create_directory(arguments, jobTests);
    output_create_files(arguments, jobTests);

    int exitStatus = run_tests(arguments, jobTests);

    test_destroy(arguments, jobTests);

    error_test_exit(exitStatus);
}

Arguments* test_get_arguments(int argc, char* argv[]) {
    Arguments* arguments = arguments_get(argc, argv);

    return arguments;
}

HashTable* test_get_jobs(Arguments* arguments) {
    HashTable* jobTests = job_get_tests(arguments);

    return jobTests;
}

void test_destroy(Arguments* arguments, HashTable* jobTests) {
    arguments_destroy(arguments);
    hash_table_destroy(jobTests);
}
