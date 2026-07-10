/*
 * ============================================================================
 *
 *       Filename:  response.h
 *
 *    Description:  Header file for response.c
 *
 *        Version:  1.0
 *        Created:  21/10/23 22:48:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4 
 *
 * ============================================================================
 */
#ifndef C_RESPONSE_H
#define C_RESPONSE_H

#define SELL_LISTED ":listed"
#define SELL_SUCCESS ":sold"
#define SELL_FAIL ":unsold"
#define NAME_FIELD 1

#define BID_PLACED ":bid"
#define BID_FAIL ":outbid"
#define BID_WON ":won"

#include "cClient.h"

/* response_start
 * ---------------
 * Starts client thread waiting for server response and 
 * parses command on arrival.
 * 
 * Parameters:
 *  client: Client information used to manage state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits when EOF is detected on server read.
 */
void response_start(Client* client);

/* response_command
 * ---------------
 * Parses server response and gets response type.
 * 
 * Parameters:
 *  client: Client information used to manage state.
 *  response: The server response.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void response_command(Client* client, char* response);

/* response_sell
 * ---------------
 * Checks if a sell response type was received and handles
 * respective sell conditions.
 * 
 * Parameters:
 *  client: Client information used to manage state.
 *  response: The server response type.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void response_sell(Client* client, char* responseType);

/* response_bid
 * ---------------
 * Checks if a bid response type was received and handles
 * respective bid conditions.
 * 
 * Parameters:
 *  client: Client information used to manage state.
 *  response: The server response type.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void response_bid(Client* client, char* responseType);

/* response_sell_add
 * ---------------
 * Thread safe increment to client sell variable using mutex.
 * 
 * Parameters:
 *  client: Client information used to manage state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 *
 * REF: Used documentation from 'man pthread_mutex_lock'
 */
void response_sell_add(Client* client);

/* response_sell_remove
 * ---------------
 * Thread safe decrement to client sell variable using mutex.
 * 
 * Parameters:
 *  client: Client information used to manage state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 *
 * REF: Used documentation from 'man pthread_mutex_lock'
 */
void response_sell_remove(Client* client);

/* response_bid_add
 * ---------------
 * Thread safe increment to client bid variable using mutex.
 * 
 * Parameters:
 *  client: Client information used to manage state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 *
 * REF: Used documentation from 'man pthread_mutex_lock'
 */
void response_bid_add(Client* client);

/* response_bid_remove
 * ---------------
 * Thread safe decrement to client bid variable using mutex.
 * 
 * Parameters:
 *  client: Client information used to manage state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 *
 * REF: Used documentation from 'man pthread_mutex_lock'
 */
void response_bid_remove(Client* client);

#endif
