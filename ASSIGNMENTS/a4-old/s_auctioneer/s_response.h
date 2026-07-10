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

#include <stdbool.h>

#include "s_item.h"
#include "s_client.h"

void response_start(Client* client);

void response_command(Client* client, char* response);

bool response_compare(char* response, char* command);

void response_sell(Client* client, char** responseFields);

Item* response_sell_get_item(Client* client, char** responseFields);

void response_bid(Client* client, char** responseFields);

Item* response_bid_get_item(Client* client, char** responseFields);

void response_list(Client* client, char** responseFields);

bool response_check_field_length(char** responseFields, int length);

long response_check_field_int(char* responseField);
#endif
