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

#include "c_client.h"

void response_start(Client* client);

void response_command(Client* client, char* response);

void response_sell(Client* client, char* responseType);

void response_bid(Client* client, char* responseType);

void response_sell_add(Client* client);

void response_sell_remove(Client* client);

void response_bid_add(Client* client);

void response_bid_remove(Client* client);

#endif
