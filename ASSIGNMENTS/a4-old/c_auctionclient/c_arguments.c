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

#include "c_arguments.h"
#include "c_error.h"

Arguments* arguments_create() {
    Arguments* arguments = (Arguments*) malloc(sizeof(Arguments));

    arguments->portNumber = "";

    return arguments;
}

Arguments* arguments_get(int argc, char* argv[]) {
    Arguments* arguments = arguments_create();

    // Disable default error output from getopt_long
    opterr = 0;

    while(true) {
        // Create command line arguments 
        static struct option ArgumentOptions[] = {
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
        // Port Number is the first mandatory argument
        arguments->portNumber = commandArgument;
    } 
}

void arguments_check_validity_mandatory(Arguments* arguments) {
    // If portnumber is NULL argument has not been set.
    if (!arguments->portNumber[0]) {
        arguments_destroy(arguments);
        error_arguments_invalid();
    }
}

void arguments_print(Arguments* arguments) {
    printf("Port Number: %s\n", arguments->portNumber);
}

void arguments_destroy(void* object) {
    if (object == NULL) {
        return;
    }

    Arguments* arguments = (Arguments*) object;

    free(arguments);
}
