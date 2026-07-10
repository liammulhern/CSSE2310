/*
 * ============================================================================
 *
 *       Filename:  exit.h
 *
 *    Description:  Header file for exit.c
 *
 *        Version:  1.0
 *        Created:  22/10/23 14:37:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4 
 *
 * ============================================================================
 */
#ifndef S_ERROR_H
#define S_ERROR_H

#define ERR_ARGS 3
#define ERR_SERVER 14

/* error_arguments_invalid
 * ---------------
 * Exits with ERR_ARGS if the command line arguments are invalid.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_ARGS.
 */
void error_arguments_invalid();

/* error_server_invalid
 * ---------------
 * Exits with ERR_SERVER if the server could not start.
 * 
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits with status ERR_SERVER.
 */
void error_server_invalid();

#endif
