/*
 * ============================================================================
 *
 *       Filename:  s_item.h
 *
 *    Description:  Header file for s_item.c
 *
 *        Version:  1.0
 *        Created:  23/10/23 20:31:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4 
 *
 * ============================================================================
 */
#ifndef S_ITEM_H
#define S_ITEM_H

#define NO_ID -1

#include <stdbool.h>
#include <stdio.h>

#include "sClient.h"

/* Item
 * ---------------
 * Auction Item data structure
 * 
 * Parameters:
 *  name: The name of the item.
 *  price: The current bidding price of the item.
 *  reserve: The initial reserve price of the item.
 *  expiry: The expiry time of the item.
 *  sellerId: The client ID of the seller.
 *  bidderId: The client ID of the current highest bidder.
 *
 */
typedef struct Item {
    char* name;
    unsigned int price;
    unsigned int reserve;
    double expiry;
    size_t sellerId;
    size_t bidderId;
} Item;

/* item_create
 * ---------------
 * Initialises and allocates item data structure settings defaults.
 * 
 * Parameters:
 *  name: The name of the item.
 *  price: The current bidding price of the item.
 *  reserve: The initial reserve price of the item.
 *  expiry: The expiry time of the item.
 *  sellerId: The client ID of the seller.
 *  bidderId: The client ID of the current highest bidder.
 *
 * Returns:
 *  Pointer to allocated item struct.
 * 
 * Errors:
 *  None
 */
Item* item_create(
        char* name,
        unsigned int price,
        unsigned int reserve,
        unsigned int time,
        size_t sellerId,
        size_t bidderId);

/* item_has_bidder
 * ---------------
 * Checks if an item has a current bidder, Returns true if it does.
 * 
 * Parameters:
 *  item: The item to check for a bidder.
 *
 * Returns:
 *  True if item has bidder, false otherwise.
 * 
 * Errors:
 *  None
 */
bool item_has_bidder(Item* item);

/* item_compare
 * ---------------
 * Compares two item objects by their names.
 * 
 * Parameters:
 *  object1: The first item data structure.
 *  object2: The second item data structure.
 *
 * Returns:
 *  True if the item names are the same, false otherwise.
 * 
 * Errors:
 *  None
 */
bool item_compare(void* object1, void* object2);

/* item_print
 * ---------------
 * Print the item information to the terminal.
 * 
 * Parameters:
 *  object: The item to print.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void item_print(void* object);

/* item_destroy
 * ---------------
 * Destorys and free memory of item.
 * 
 * Parameters:
 *  object: The item object to destory.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void item_destroy(void* object);

#endif
