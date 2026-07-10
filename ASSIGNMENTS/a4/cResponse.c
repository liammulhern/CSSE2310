/*
 * ============================================================================
 *
 *       Filename:  response.c
 *
 *    Description:  Handles server responses to the client.
 *
 *        Version:  1.0
 *        Created:  21/10/23 22:47:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4
 *
 * ============================================================================
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <csse2310a3.h>
#include <csse2310a4.h>

#include "cResponse.h"
#include "cRequest.h"

void response_start(Client* client) {
    while (1) {
        char* response = read_line(client->serverRead);

        // Check for EOF
        if (response == NULL) {
            break;
        }
         
        // Print server response to terminal
        printf("%s\n", response);
        fflush(stdout);

        response_command(client, response);

        free(response);
    }
}

void response_command(Client* client, char* response) {
    char** responseFields = split_by_char(response, 
            FIELD_SPLIT, ALL_FIELDS);
    char* responseType = responseFields[0];
    
    response_sell(client, responseType);
    response_bid(client, responseType);

    free(responseFields);
}

void response_sell(Client* client, char* responseType) {
    if (request_compare(responseType, SELL_LISTED)) {
        response_sell_add(client);
    } else if (request_compare(responseType, SELL_SUCCESS)) {
        response_sell_remove(client);
    } else if (request_compare(responseType, SELL_FAIL)) {
        response_sell_remove(client);
    }
}

void response_bid(Client* client, char* responseType) {
    if (request_compare(responseType, BID_PLACED)) {
        response_bid_add(client);
    } else if (request_compare(responseType, BID_FAIL)) {
        response_bid_remove(client);
    } else if (request_compare(responseType, BID_WON)) {
        response_bid_remove(client);
    }
}

void response_sell_add(Client* client) {
    pthread_mutex_lock(client->mutex);
    client->listedItems++;
    pthread_mutex_unlock(client->mutex);
}

void response_sell_remove(Client* client) {
    pthread_mutex_lock(client->mutex);
    client->listedItems--;
    pthread_mutex_unlock(client->mutex);
}

void response_bid_add(Client* client) {
    pthread_mutex_lock(client->mutex);
    client->biddingItems++;
    pthread_mutex_unlock(client->mutex);
}

void response_bid_remove(Client* client) {
    pthread_mutex_lock(client->mutex);
    client->biddingItems--;
    pthread_mutex_unlock(client->mutex);
}
