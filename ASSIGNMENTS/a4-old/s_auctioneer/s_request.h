/*
 * ============================================================================
 *
 *       Filename:  s_request.h
 *
 *    Description:  Header file for s_request.c
 *
 *        Version:  1.0
 *        Created:  23/10/23 22:33:08
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4 
 *
 * ============================================================================
 */
#ifndef S_REQUEST_H
#define S_REQUEST_H

#include "s_client.h"
#include "s_item.h"

void request_bid_item(Client* client, Item* item);

void request_sell_item(Client* client, Item* item);

void request_won_item(Client* client, Item* item);

void request_sold_item(Client* client, Item* item);

void request_unsold_item(Client* client, Item* item);

void request_outbid_item(Client* client, Item* oldItem, Item* newItem);

void request_list(Client* client);

void request_rejected(Client* client);

void request_invalid(Client* client);

#endif 
