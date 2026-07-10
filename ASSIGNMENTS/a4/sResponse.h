/*
 * ============================================================================
 *
 *       Filename:  s_response.h
 *
 *    Description:  Header file for s_response.c
 *
 *        Version:  1.0
 *        Created:  23/10/23 21:29:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4
 *
 * ============================================================================
 */
#ifndef S_RESPONSE_H
#define S_RESPONSE_H

#define FIELD_SPLIT ' '
#define ALL_FIELDS 0
#define SELL_FIELD_NAME 1
#define SELL_FIELD_RESERVE 2
#define SELL_FIELD_TIME 3
#define SELL_FIELD_SIZE 4
#define BID_FIELD_NAME 1
#define BID_FIELD_PRICE 2
#define BID_FIELD_SIZE 3
#define LIST_FIELD_SIZE 1
#define NO_ID -1

#include <stdbool.h>

#include "sItem.h"
#include "sClient.h"

/* response_start
 * ---------------
 * Start reading client responses to server requests until
 * client disconnects.
 * 
 * Parameters:
 *  client: The data structure used to manage server clients.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits thread when client disconnects.
 */
void response_start(Client* client);

/* response_command
 * ---------------
 * Parses client response and gets response type.
 * 
 * Parameters:
 *  client: Client information used to manage state.
 *  response: The client response.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void response_command(Client* client, char* response);

/* response_compare
 * ---------------
 * Compare response with expected command and determine if
 * they match.
 * 
 * Parameters:
 *  response: The response to match.
 *  command: The command to match.
 *
 * Returns:
 *  True if the response and command match, false otherwise.
 * 
 * Errors:
 *  None
 */
bool response_compare(char* response, char* command);

/* response_sell
 * ---------------
 * Checks if a sell response type was received and handles
 * respective sell conditions.
 * 
 * Parameters:
 *  client: Client information used to manage state.
 *  responseFields: The client response fields.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void response_sell(Client* client, char** responseFields);

/* response_sell_get_item
 * ---------------
 * Attempts to create an item from client sell request.
 * 
 * Parameters:
 *  client: Client information used to manage state.
 *  responseFields: The client response fields.
 *
 * Returns:
 *  Item if client response was valid, NULL otherwise.
 * 
 * Errors:
 *  None
 */
Item* response_sell_get_item(Client* client, char** responseFields);

/* response_bid
 * ---------------
 * Checks if a bid response type was received and handles
 * respective bid conditions.
 * 
 * Parameters:
 *  client: Client information used to manage state.
 *  responseFields: The client response fields.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void response_bid(Client* client, char** responseFields);

/* response_bid_get_item
 * ---------------
 * Attempts to create an item from client bid request.
 * 
 * Parameters:
 *  client: Client information used to manage state.
 *  responseFields: The client response fields.
 *
 * Returns:
 *  Item if client response was valid, NULL otherwise.
 * 
 * Errors:
 *  None
 */
Item* response_bid_get_item(Client* client, char** responseFields);

/* response_list
 * ---------------
 * Attempts to print list of auction items from client request.
 * 
 * Parameters:
 *  client: Client information used to manage state.
 *  responseFields: The client response fields.
 * 
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void response_list(Client* client, char** responseFields);

/* response_check_field_length
 * ---------------
 * Check if the response field length matches the expected length.
 * 
 * Parameters:
 *  client: Client information used to manage state.
 *  responseFields: The client response fields.
 *  
 * Returns:
 *  True if field length does not match expected length, false otherwise.
 * 
 * Errors:
 *  None
 */
bool response_check_field_length(char** responseFields, int length);

/* response_check_field_int
 * ---------------
 * Check if the response field number is valid and return number.
 * 
 * Parameters:
 *  responseField: The client response field containing number.
 *
 * Returns:
 *  The number if it is valid, -1 otherwise.
 * 
 * Errors:
 *  None
 */
long response_check_field_int(char* responseField);

#endif
