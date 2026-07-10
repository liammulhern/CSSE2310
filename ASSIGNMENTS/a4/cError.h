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

/* error_port_invalid
 * ---------------
 * Exits the program with ERR_PORT if client cannot connect to server.
 * 
 * Parameters:
 *  port: The port the client attempted to connect to.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_PORT.
 */
void error_port_invalid(char* port);

/* error_server_closed
 * ---------------
 * Exits the program with ERR_SERVER_CLOSED if client's connection is lost.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_SERVER_CLOSED.
 */
void error_server_closed();

/* error_client_closed
 * ---------------
 * Exits program with ERR_AUCT_IN_PROG if client disconnects from server
 * whilst in progress.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_AUCT_IN_PROG.
 */
void error_client_closed();

/* error_quit
 * ---------------
 * Exits program with ERR_QUIT if client successfuly disconnects from server.
 * 
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_QUIT.
 */
void error_quit();

#endif
