/*
 * ============================================================================
 *
 *       Filename:  error.c
 *
 *    Description:  Exit point for server
 *
 *        Version:  1.0
 *        Created:  22/10/23 14:36:46
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

#include "sError.h"

void error_arguments_invalid() {
    fprintf(stderr, "Usage: auctioneer [--maxclients num-connections] "
            "[--listenon portnum]\n");

    exit(ERR_ARGS);
}

void error_server_invalid() {
    fprintf(stderr, "auctioneer: unable to listen on port\n");

    exit(ERR_SERVER);
}
