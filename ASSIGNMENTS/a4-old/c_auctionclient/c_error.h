/*
 * ============================================================================
 *
 *       Filename:  error.h
 *
 *    Description:  Header file for error.c
 *
 *        Version:  1.0
 *        Created:  17/09/23 16:41:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A3
 *
 * ============================================================================
 */
#ifndef C_ERROR_H
#define C_ERROR_H

#define ERR_CMD_ARGS 18
#define ERR_PORT 1
#define ERR_SERVER_CLOSED 20
#define ERR_AUCT_IN_PROG 16
#define ERR_QUIT 0

/* error_arguments_invalid()
 * ---------------
 * Prints usage error to stderr and exits with status ERR_CMD_ARGS.
 * 
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_CMD_ARGS.
 */
void error_arguments_invalid();

void error_port_invalid(char* port);

void error_server_closed();

void error_client_closed();

void error_quit();

#endif
