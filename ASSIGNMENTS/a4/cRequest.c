/*
 * ============================================================================
 *
 *       Filename:  request.c
 *
 *    Description:  Handles client request and parsing.
 *
 *        Version:  1.0
 *        Created:  21/10/23 15:04:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4
 *
 * ============================================================================
 */
#include <stdlib.h>
#include <string.h>
#include <csse2310a3.h>
#include <csse2310a4.h>
#include <ctype.h>

#include "cRequest.h"

void request_start(Client* client) {
    while (1) {
        char* request = read_line(stdin);
    
        // Check for EOF
        if (request == NULL) {
            client_close(client);
        }
        
        // Check for comments
        if (request_is_comment(request)) {
            free(request);
            continue;
        }

        // Check for quit command
        if (request_is_quit(request)) {
            client_quit(client);
            continue;
        }

        request_command(client, request);
        free(request);
    }
}

void request_command(Client* client, char* request) {
    fprintf(client->serverWrite, "%s\n", request);
    fflush(client->serverWrite); 
}

bool request_is_comment(char* request) {
    int i = 0;

    while (request[i] != '\0') {
        if (isspace(request[i])) {
            // Check for whitespace and comments
            i++;
        } else if (request[i] == COMMENT) {
            // Check if # is first character
            return true;
        } else {
            return false;
        }
    }

    return true;
}

bool request_is_quit(char* request) {
    return request_compare(request, QUIT);
}

bool request_compare(char* request, char* command) {
    return strcmp(request, command) == 0;
}
