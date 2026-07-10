/*
 * ============================================================================
 *
 *       Filename:  request.h
 *
 *    Description:  Header file for request.c
 *
 *        Version:  1.0
 *        Created:  21/10/23 15:05:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4
 *
 * ============================================================================
 */
#ifndef C_REQUEST_H
#define C_REQUEST_H

#define FIELD_SPLIT ' '
#define ALL_FIELDS 0

#define COMMENT '#'
#define QUIT "quit"

#include <stdbool.h>

#include "c_client.h"

void request_start(Client* client);

void request_command(Client* client, char* request);

bool request_is_comment(char* request);

bool request_is_quit(char* request);

bool request_compare(char* request, char* command);

#endif
