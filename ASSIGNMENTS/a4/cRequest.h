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

#include "cClient.h"

/* request_start
 * ---------------
 * Read stdin input from user and send requests to server after
 * parsing comments and empty lines.
 * 
 * Parameters:
 *  client: Client information used to manage state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits via client_close() if EOF is detected on stdin.
 */
void request_start(Client* client);

/* request_command
 * ---------------
 * Writes and flushes user input to the server.
 * 
 * Parameters:
 *  client: Client information used to manage state.
 *  request: The stdin input sent to the server.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void request_command(Client* client, char* request);

/* request_is_comment
 * ---------------
 * Check if stdin input is a comment or white space.
 * 
 * Parameters:
 *  request: The stdin input from client.
 *
 * Returns:
 *  True if request is comment or whitespace, false otherwise.
 * 
 * Errors:
 *  None
 */
bool request_is_comment(char* request);

/* request_is_quit
 * ---------------
 * Check if request is quit command and return true if it is.
 * 
 * Parameters:
 *  request: The stdin input from client.
 *
 * Returns:
 *  True if request is quit command, false otherwise.
 * 
 * Errors:
 *  None
 */
bool request_is_quit(char* request);

/* request_compare
 * ---------------
 * Compares request to expected commands, and return true if they match.
 * 
 * Parameters:
 *  request: The stdin input from client.
 *  command: Command to compare request to.
 *
 * Returns:
 *  True if command and request match, false otherwise.
 * 
 * Errors:
 *  None
 */
bool request_compare(char* request, char* command);

#endif
