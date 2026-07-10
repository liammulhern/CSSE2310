/*
 * ============================================================================
 *
 *       Filename:  arguments.c
 *
 *    Description:  Parses command line arguments.
 *
 *        Version:  1.0
 *        Created:  22/10/23 11:50:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4 
 *
 * ============================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#include "sArguments.h"
#include "sError.h"

Arguments* arguments_create() {
    Arguments* arguments = (Arguments*) malloc(sizeof(Arguments));

    arguments->portNumber = "";
    arguments->maxConnections = -1;

    return arguments;
}

Arguments* arguments_get(int argc, char* argv[]) {
    Arguments* arguments = arguments_create();

    // Disable default error output from getopt_long
    opterr = 0;

    while (1) {
        // Create command line arguments 
        static struct option argumentOptions[] = {
                {"maxclients", required_argument, 0, CMD_CONNECTION},
                {"listenon", required_argument, 0, CMD_PORT},
                {0, 0, 0, 0}};

        // Iterate over command line arguments using long options 
        int commandType = getopt_long(argc, argv, "", argumentOptions, NULL);

        // Check if all command line arguments have been read 
        if (commandType == -1) {
            if (argc != optind) {
                arguments_destroy(arguments);
                error_arguments_invalid();
            }
            break;
        }

        // The optarg variable is set by getopt_long and represents 
        // the argument following the command.
        arguments_check_validity(arguments, commandType, optarg);
    }
    arguments_set_default(arguments);

    return arguments;
}

void arguments_check_validity(
        Arguments* arguments, 
        int commandType, 
        char* commandArgument) {

    switch (commandType) {
        case CMD_PORT:
            // Set valid port number 
            arguments->portNumber = arguments_check_validity_port(arguments, 
                    commandArgument);
            break;
        case CMD_CONNECTION:
            // Set valid max connection 
            arguments->maxConnections = 
                    arguments_check_validity_max_connection(arguments, 
                    commandArgument);
            break;
        case CMD_ERROR:
        default:
            // If unknown argument is present free memory and exit
            arguments_destroy(arguments);
            error_arguments_invalid();
            break;
    }
}

char* arguments_check_validity_port(
        Arguments* arguments, 
        char* commandArgument) {
    
    // If argumentError flag changes from 0 an error occured when validating
    int argumentError = 0;

    argumentError += argument_check_repeated_command_string(
            arguments->portNumber);
    size_t portNumber = arguments_check_valid_number(commandArgument);
    
    // Check for port argument errors
    if (argumentError 
            || portNumber == -1 
            || (portNumber < MIN_PORT && portNumber != 0)
            || portNumber > MAX_PORT) {
        arguments_destroy(arguments);
        error_arguments_invalid();
    }
    
    return commandArgument;
}

size_t arguments_check_validity_max_connection(
        Arguments* arguments,
        char* commandArgument) {

    // If argumentError flag changes from 0 an error occured when validating
    int argumentError = 0;

    argumentError += argument_check_repeated_command_int(
            arguments->maxConnections);
    long connectionNumber = arguments_check_valid_number(commandArgument);

    // Check if max connection is valid
    if (argumentError || connectionNumber == -1) {
        arguments_destroy(arguments);
        error_arguments_invalid();
    }
    
    return connectionNumber;
}

bool argument_check_repeated_command_string(char* argument) {
    return argument[0];
}

bool argument_check_repeated_command_int(int argument) {
    return argument != -1;
}

long arguments_check_valid_number(char* argument) {
    char* numberInvalidFlag = NULL;

    // Convert string to long in base 10
    long number = strtol(argument, &numberInvalidFlag, 10);

    // Check for errors in strtol function
    if (errno != 0 
            || numberInvalidFlag == argument 
            || *numberInvalidFlag != '\0'
            || number < 0) {
        return -1;
    }

    return number;
}

void arguments_set_default(Arguments* arguments) {
    if (!arguments->portNumber[0]) {
        arguments->portNumber = "0";
    }

    if (arguments->maxConnections <= 0) {
        arguments->maxConnections = INT_MAX;
    }
}

void arguments_print(Arguments* arguments) {
    printf("Port Number: %s\nMax Clients: %ld\n", arguments->portNumber, 
            arguments->maxConnections);
}

void arguments_destroy(void* object) {
    if (object == NULL) {
        return;
    }

    Arguments* arguments = (Arguments*) object;
    free(arguments);
}
