/*
 * ============================================================================
 *
 *       Filename:  s_request.c
 *
 *    Description:  Server request handler for client messages.
 *
 *        Version:  1.0
 *        Created:  23/10/23 22:32:40
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

#include "s_request.h"

void request_bid_item(Client* client, Item* item) {
    if (client == NULL || item == NULL || client->clientWrite == NULL) {
        return;
    }

    fprintf(client->clientWrite, ":bid %s\n", item->name);
    fflush(client->clientWrite);
}

void request_sell_item(Client* client, Item* item) {
    if (client == NULL || item == NULL || client->clientWrite == NULL) {
        return;
    }

    fprintf(client->clientWrite, ":listed %s\n", item->name);
    fflush(client->clientWrite);
}

void request_won_item(Client* client, Item* item) {
    if (client == NULL || item == NULL || client->clientWrite == NULL) {
        printf("NO CLIENT\n");
        return;
    }

    fprintf(client->clientWrite, ":won %s %u\n", item->name, item->price);
    fflush(client->clientWrite);
}

void request_sold_item(Client* client, Item* item) {
    if (client == NULL || item == NULL || client->clientWrite == NULL) {
        printf("NO CLIENT\n");
        return;
    }

    fprintf(client->clientWrite, ":sold %s %u\n", item->name, item->price);
    fflush(client->clientWrite);
}

void request_unsold_item(Client* client, Item* item) {
    if (client == NULL || item == NULL || client->clientWrite == NULL) {
        printf("NO CLIENT\n");
        return;
    }

    fprintf(client->clientWrite, ":unsold %s\n", item->name);
    fflush(client->clientWrite);
}

void request_outbid_item(Client* client, Item* oldItem, Item* newItem) {
    if (client == NULL 
            || oldItem == NULL 
            || newItem == NULL 
            || client->clientWrite == NULL) {
        return;
    }

    fprintf(client->clientWrite, ":outbid %s %u\n", oldItem->name, 
            newItem->price);
    fflush(client->clientWrite);
}

void request_list(Client* client) {
    if (client == NULL || client->clientWrite == NULL) {
        return;
    }        

    fprintf(client->clientWrite, ":list ");
    fflush(client->clientWrite);

    List* items = client->server->items;
    ListNode* currentItem = items->head;

    while (currentItem != NULL) {
        Item* item = currentItem->value;
        int remaining = (int) (item->expiry - get_time_ms());

        fprintf(client->clientWrite, "%s %u %u %u| ", item->name, item->reserve, 
            item->price, remaining);
        fflush(client->clientWrite);

        currentItem = currentItem->next;
    }

    fprintf(client->clientWrite, "\n");
    fflush(client->clientWrite);
}

void request_rejected(Client* client) {
    if (client == NULL || client->clientWrite == NULL) {
        return;
    }
    
    fprintf(client->clientWrite, ":rejected\n");
    fflush(client->clientWrite);
}

void request_invalid(Client* client) {
    if (client == NULL || client->clientWrite == NULL) {
        return;
    }

    fprintf(client->clientWrite, ":invalid\n");
    fflush(client->clientWrite);
}
