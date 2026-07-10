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

#include "c_error.h"

void error_arguments_invalid() {
    fprintf(stderr, "Usage: auctionclient portno\n");

    exit(ERR_CMD_ARGS);
}

void error_port_invalid(char* portNumber) {
    fprintf(stderr, "auctionclient: unable to connect to port %s\n", portNumber);
    free(portNumber);

    exit(ERR_PORT);
}

void error_server_closed() {
    fprintf(stderr, "auctionclient: server has gone away\n");

    exit(ERR_SERVER_CLOSED);
}

void error_client_closed() {
    fprintf(stderr, "Quitting with auction(s) still in progress\n");

    exit(ERR_AUCT_IN_PROG);
}

void error_quit() {
    exit(ERR_QUIT);
}
