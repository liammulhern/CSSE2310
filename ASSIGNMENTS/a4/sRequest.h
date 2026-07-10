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

#include "sClient.h"
#include "sItem.h"

/* request_bid_item
 * ---------------
 * Send successful bid item request to client.
 * 
 * Parameters:
 *  client: The client to send the message.
 *  item: The valid item that was bid.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void request_bid_item(Client* client, Item* item);

/* request_sell_item
 * ---------------
 * Send successful sell listed item request to client.
 * 
 * Parameters:
 *  client: The client to send the message.
 *  item: The valid item that was listed.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void request_sell_item(Client* client, Item* item);

/* request_won_item
 * ---------------
 * Send successful won item request to client.
 * 
 * Parameters:
 *  client: The client to send the message.
 *  item: The valid item that was won.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void request_won_item(Client* client, Item* item);

/* request_sold_item
 * ---------------
 * Send successful sold item request to client.
 * 
 * Parameters:
 *  client: The client to send the message.
 *  item: The valid item that was sold.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void request_sold_item(Client* client, Item* item);

/* request_unsold_item
 * ---------------
 * Send successful unsold item request to client.
 * 
 * Parameters:
 *  client: The client to send the message.
 *  item: The valid item that was unsold.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void request_unsold_item(Client* client, Item* item);

/* request_outbid_item
 * ---------------
 * Send successful outbid item request to client.
 * 
 * Parameters:
 *  client: The client to send the message.
 *  oldItem: The item that was outbid.
 *  newItem: The new item that was bid.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void request_outbid_item(Client* client, Item* oldItem, Item* newItem);

/* request_list
 * ---------------
 * Send successful list item request to client.
 * 
 * Parameters:
 *  client: The client to send the message.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void request_list(Client* client);

/* request_rejected
 * ---------------
 * Send rejected request to client.
 * 
 * Parameters:
 *  client: The client to send the message.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void request_rejected(Client* client);

/* request_invalid
 * ---------------
 * Send invalid request to client.
 * 
 * Parameters:
 *  client: The client to send the message.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void request_invalid(Client* client);

#endif 
