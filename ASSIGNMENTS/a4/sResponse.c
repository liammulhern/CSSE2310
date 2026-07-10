/*
 * ============================================================================
 *
 *       Filename:  s_response.c
 *
 *    Description:  Handles reponses to client requests.
 *
 *        Version:  1.0
 *        Created:  23/10/23 21:28:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4 
 *
 * ============================================================================
 */
#include <stdlib.h>
#include <csse2310a3.h>
#include <csse2310a4.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#include "sResponse.h"
#include "sRequest.h"

void response_start(Client* client) {
    while (1) {
        char* response = read_line(client->clientRead);

        // Check for EOF
        if (response == NULL) {
            break;
        }
         
        response_command(client, response);
        free(response);
    } 

    client_disconnect(client);
}

void response_command(Client* client, char* response) {
    char* responseCopy = strdup(response);

    char** responseFields = split_by_char(responseCopy, 
            FIELD_SPLIT, ALL_FIELDS);
    char* responseType = responseFields[0];

    // Check if responseType is valid command type
    if (response_compare(responseType, "sell")) {
        response_sell(client, responseFields);
    } else if (response_compare(responseType, "bid")) {
        response_bid(client, responseFields);
    } else if (response_compare(responseType, "list")) {
        response_list(client, responseFields);
    } else {
        request_invalid(client);
    }

    free(responseFields);
    free(responseCopy);
}

bool response_compare(char* response, char* command) {
    return strcmp(response, command) == 0;
}

void response_sell(Client* client, char** responseFields) {
    pthread_mutex_lock(client->server->mutex);
    client->server->sellTotal++;
    pthread_mutex_unlock(client->server->mutex);

    Item* item = response_sell_get_item(client, responseFields); 

    if (item == NULL) {
        request_invalid(client);
        return;
    }

    // Check if item is already listed
    if (list_find(client->server->items, item) != NULL) {
        request_rejected(client);
        return;
    }

    list_append(client->server->items, item);
    request_sell_item(client, item);

    pthread_mutex_lock(client->server->mutex);
    client->server->sellSuccessful++;
    pthread_mutex_unlock(client->server->mutex);
}

Item* response_sell_get_item(Client* client, char** responseFields) {
    int sellInvalid = response_check_field_length(responseFields, 
            SELL_FIELD_SIZE);

    if (sellInvalid) {
        return NULL;
    }

    long reserve = response_check_field_int(
            responseFields[SELL_FIELD_RESERVE]);
    long time = response_check_field_int(
            responseFields[SELL_FIELD_TIME]);
    
    if (reserve < 0 || time < 1) {
        return NULL;
    }

    // Create item with a current bid of 0, current client as seller 
    // and no current bidder.
    Item* item = item_create(responseFields[SELL_FIELD_NAME], 
            0, reserve, time, client->id, NO_ID);

    return item;
}

void response_bid(Client* client, char** responseFields) {
    pthread_mutex_lock(client->server->mutex);
    client->server->bidTotal++;
    pthread_mutex_unlock(client->server->mutex);

    Item* tempBid = response_bid_get_item(client, responseFields);

    if (tempBid == NULL) {
        request_invalid(client);
        return;
    }

    Item* currentBid = list_find(client->server->items, tempBid);
    
    pthread_mutex_lock(client->server->mutex);

    // Check if bid should be rejected
    if (currentBid == NULL 
            || currentBid->bidderId == client->id
            || currentBid->sellerId == client->id
            || tempBid->price <= currentBid->price
            || tempBid->price < currentBid->reserve) {
        item_destroy(tempBid);
        pthread_mutex_unlock(client->server->mutex);
        request_rejected(client);
        return;
    }

    pthread_mutex_unlock(client->server->mutex);

    Client* bidder = client_from_id(client->server, 
            currentBid->bidderId);

    pthread_mutex_lock(client->server->mutex);

    // Send server client requests
    request_outbid_item(bidder, currentBid, tempBid);
    request_bid_item(client, tempBid);

    // Update the current bid values
    currentBid->price = tempBid->price;
    currentBid->bidderId = tempBid->bidderId;

    item_destroy(tempBid);
    client->server->bidSuccessful++;

    pthread_mutex_unlock(client->server->mutex);
}

Item* response_bid_get_item(Client* client, char** responseFields) {
    int bidInvalid = response_check_field_length(responseFields, 
            BID_FIELD_SIZE);

    if (bidInvalid) {
        return NULL;
    }

    long price = response_check_field_int(
            responseFields[BID_FIELD_PRICE]);
    
    if (price < 1) {
        return NULL;
    }

    // Create temporary item used to update current with new 
    // bidder as current client
    Item* item = item_create(responseFields[BID_FIELD_NAME], 
            price, 0, 0, NO_ID, client->id);

    return item;
}

void response_list(Client* client, char** responseFields) {
    int listInvalid = response_check_field_length(responseFields, 
            LIST_FIELD_SIZE);

    if (listInvalid) {
        request_invalid(client);
        return;
    }

    request_list(client);
}

bool response_check_field_length(char** responseFields, int length) {
    int i = 0;

    // Iterate over reponse fields and count length
    while (responseFields[i] != NULL) {
        if (i > length) {
            return true;
        }

        i++;
    }

    return i != length;
}

long response_check_field_int(char* responseField) {
    char* responseFieldEnd;

    long field = strtol(responseField, &responseFieldEnd, 10);

    // Check if errors occured when parsing number
    if ((errno == ERANGE && (field == LONG_MAX || field == LONG_MIN))
            || (errno != 0 && field == 0)
            || responseFieldEnd == responseField 
            || *responseFieldEnd != '\0') {
        return -1;
    }

    return field;
}
