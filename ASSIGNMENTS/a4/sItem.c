/*
 * ============================================================================
 *
 *       Filename:  s_item.c
 *
 *    Description:  Item handler for bidding and selling
 *
 *        Version:  1.0
 *        Created:  23/10/23 20:30:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4 
 *
 * ============================================================================
 */
#include <stdlib.h>
#include <csse2310a4.h>
#include <string.h>

#include "sItem.h"

Item* item_create(
        char* name,
        unsigned int price,
        unsigned int reserve,
        unsigned int time,
        size_t sellerId,
        size_t bidderId) {
    Item* item = (Item*) malloc(sizeof(Item));

    item->sellerId = sellerId;
    item->bidderId = bidderId;
    item->name = strdup(name);
    item->price = price;
    item->reserve = reserve;
    item->expiry = get_time_ms() + (double) time;

    return item;
}

bool item_has_bidder(Item* item) {
    return item->bidderId != NO_ID;
}

bool item_compare(void* object1, void* object2) {
    Item* item1 = (Item*) object1;
    Item* item2 = (Item*) object2;

    return strcmp(item1->name, item2->name) == 0;
}

void item_print(void* object) {
    Item* item = (Item*) object;
    
    int remaining = (int)(item->expiry - get_time_ms());
    
    printf("%s Res: %u Price: %u Remaining: %u Bidder: %ld, Seller %ld\n", 
            item->name, item->reserve, item->price, remaining, 
            item->bidderId, item->sellerId);
}

void item_destroy(void* object) {
    Item* item = (Item*) object;

    free(item->name);
    free(item);
}
