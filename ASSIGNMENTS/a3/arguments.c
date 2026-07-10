/*
 * ============================================================================
 *
 *       Filename:  arguments.c
 *
 *    Description:  Gets command line arguments from terminal and 
 *                  checks validity before saving arguments to struct.
 *
 *        Version:  1.0
 *        Created:  17/09/23 11:10:56
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
#include <getopt.h>
#include <errno.h>
#include <string.h>

#include "arguments.h"
#include "error.h"

Arguments* arguments_create() {
    Arguments* arguments = (Arguments*) malloc(sizeof(Arguments));

    arguments->testOutputDirectory = "";
    arguments->testsRegenerate = false;
    arguments->testJobFilePath = "";
    arguments->testProgramPath = "";

    return arguments;
}

Arguments* arguments_get(int argc, char* argv[]) {
    Arguments* arguments = arguments_create();

    // Disable default error output from getopt_long
    opterr = 0;

    while(true) {
        // Create command line arguments 
        static struct option ArgumentOptions[] = {
            {"testdir", required_argument, 0, TEST_DIR},
            {"regen", no_argument, 0, TEST_REGEN},
            {0, 0, 0, 0}
        };

        // Iterate over command line arguments using long options 
        int commandType = getopt_long(argc, argv, "-", ArgumentOptions, NULL);

        // Check if all command line arguments have been read 
        if (commandType == -1) {
            break;
        }

        // The optarg variable is set by getopt_long and represents 
        // the argument following the command.
        arguments_check_validity(arguments, optind, commandType, optarg, argc);
    }

    arguments_check_validity_mandatory(arguments);
    arguments_set_default(arguments);

    return arguments;
}

void arguments_check_validity(
        Arguments* arguments, 
        int commandIndex,
        int commandType, 
        char* commandArgument,
        int commandCount) {

    int mandatoryStartIndex = commandCount - MANDATORY_ARG_COUNT;

    switch (commandType) {
        case TEST_DIR:
            // Set test output directory if valid 
            arguments->testOutputDirectory = arguments_check_validity_string(
                    arguments, arguments->testOutputDirectory, 
                    commandArgument);
            break;
        case TEST_REGEN:
            // Set test regenerate if valid 
            arguments->testsRegenerate = arguments_check_validity_int(
                    arguments, arguments->testsRegenerate, true);
            break;
        case MANDATORY:
            // Check if mandatory arguments are the trailing n arguments
            if (commandIndex > mandatoryStartIndex) {
                arguments_set_mandatory(arguments, commandIndex, 
                        commandArgument, mandatoryStartIndex);
                break;
            } 
        case CMD_ERROR:
        default:
            // If unknown argument is present free memory and exit
            arguments_destroy(arguments);
            error_arguments_invalid();
            break;
    }
}

void arguments_set_mandatory(
        Arguments* arguments, 
        int commandIndex,
        char* commandArgument,
        int mandatoryStartIndex) {

    if (commandIndex == mandatoryStartIndex + 1) {
        // Job file is the first mandatory argument
        arguments->testJobFilePath = commandArgument;
    } else if (commandIndex == mandatoryStartIndex + 2) {
        // Program file is the second mandatory argument
        arguments->testProgramPath = commandArgument;
    }
}

void arguments_check_validity_mandatory(Arguments* arguments) {
    // If test job file path or test program path is null then mandatory 
    // arguments have not been set.
    if (!arguments->testJobFilePath[0] || !arguments->testProgramPath[0]) {
        arguments_destroy(arguments);
        error_arguments_invalid();
    }
}

char* arguments_check_validity_string(
        Arguments* arguments,
        char* argument,
        char* commandArgument) {
    // If argumentError flag changes from 0 an error occured when validating
    int argumentError = 0;

    argumentError += arguments_check_repeated_command_string(argument);

    if (argumentError) {
        arguments_destroy(arguments);
        error_arguments_invalid();
    }

    return commandArgument;
}

bool arguments_check_repeated_command_string(char* argument) {
    return argument[0];
}

int arguments_check_validity_int(
        Arguments* arguments,
        int argument,
        int commandArgument) {
    // If argumentError flag changes from 0 an error occured when validating
    int argumentError = 0;

    argumentError += arguments_check_repeated_command_int(argument);

    if (argumentError) {
        arguments_destroy(arguments);
        error_arguments_invalid();
    }

    return commandArgument;
}

bool arguments_check_repeated_command_int(int argument) {
    return argument;
}

void arguments_set_default(Arguments* arguments) {
    // Check if test output directory has been set
    if (!arguments->testOutputDirectory[0]) {
        arguments->testOutputDirectory = DEFAULT_TEST_DIR;
    }
}

void arguments_print(Arguments* arguments) {
    printf("Test Output Directory: %s\n", arguments->testOutputDirectory);
    printf("Regenerate Test Files: %d\n", arguments->testsRegenerate);
    printf("Test Job File Path: %s\n", arguments->testJobFilePath);
    printf("Test Program Path: %s\n", arguments->testProgramPath);
}

void arguments_destroy(void* object) {
    if (object == NULL) {
        return;
    }

    Arguments* arguments = (Arguments*) object;

    free(arguments);
}
