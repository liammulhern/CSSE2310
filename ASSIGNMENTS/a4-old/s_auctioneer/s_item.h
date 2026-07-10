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

#include <stdbool.h>
#include <stdio.h>

#include "s_client.h"

typedef struct Item {
    char* name;
    unsigned int price;
    unsigned int reserve;
    double expiry;
    Client* seller;
    Client* bidder;
} Item;

Item* item_create(
        char* name,
        unsigned int price,
        unsigned int reserve,
        unsigned int time,
        Client* seller,
        Client* bidder);

bool item_has_bidder(Item* item);

bool item_compare(void* key1, void* key2);

void item_print(void* object);

void item_destroy(void* object);

#endif
