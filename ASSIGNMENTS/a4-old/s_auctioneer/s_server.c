/*
 * ============================================================================
 *
 *       Filename:  server.c
 *
 *    Description:  Server handler
 *
 *        Version:  1.0
 *        Created:  15/10/23 12:20:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4
 *
 * ============================================================================
 */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <csse2310a3.h>
#include <csse2310a4.h>
#include <signal.h>

#include "s_server.h"
#include "s_arguments.h"
#include "s_error.h"
#include "s_client.h"
#include "s_item.h"
#include "s_request.h"

void server_start(int argc, char* argv[]) {
    Server* server = server_create(argc, argv);

    server_print(server);
    
    server_process_items(server);
    server_process_stats(server);
    server_process_connections(server);

    server_destroy(server);
}

Server* server_create(int argc, char* argv[]) {
    // Get command line arguments
    Arguments* arguments = arguments_get(argc, argv);

    Server* server = (Server*) malloc(sizeof(Server));

    // Create mutex fo all threads to share
    pthread_mutex_t* mutex = (pthread_mutex_t*) malloc(sizeof(
                pthread_mutex_t));
    pthread_mutex_init(mutex, NULL);

    // Create signal mask for all threads to share
    sigset_t* signalMask = (sigset_t*) malloc(sizeof(sigset_t));
    sigemptyset(signalMask);
    sigaddset(signalMask, SIGHUP);
    pthread_sigmask(SIG_BLOCK, signalMask, NULL);

    // Create empty map to store clients
    Map* clients = map_create(client_compare_map, client_destroy_map, 
            client_print_map, mutex);

    // Create empty list to store items
    List* items = list_create(item_compare, item_destroy, 
            item_print, mutex);
    
    // Set server defaults
    server->listenFd = -1;
    server->port = -1;
    server->clientId = 0;
    server->completedClients = 0; 
    server->sellTotal = 0;
    server->sellSuccessful = 0;
    server->bidTotal = 0;
    server->bidSuccessful = 0;
    server->clients = clients;
    server->items = items;
    server->maxConnections = arguments->maxConnections;
    server->mutex = mutex;
    server->signalMask = signalMask;

    server_open(server, arguments->portNumber);

    arguments_destroy(arguments);

    return server;
}

void server_open(Server* server, char* port) {
    struct addrinfo* addressInfo;

    addressInfo = server_get_address(server, port);
    server_socket_connect(server, addressInfo);
}

struct addrinfo* server_get_address(Server* server, char* port) {
    struct addrinfo* addressInfo;
    struct addrinfo addressHints;
    memset(&addressHints, 0, sizeof(addressHints));

    addressHints.ai_family = AF_INET;
    addressHints.ai_socktype = SOCK_STREAM;
    addressHints.ai_flags = AI_PASSIVE;
    addressHints.ai_protocol = 0;

    int addressError = getaddrinfo(NULL, port, &addressHints, 
                &addressInfo);

    if (addressError != 0) {
        // Failed getting address info
        server_invalid(server, addressInfo);
    }

    return addressInfo;
}

void server_socket_connect(Server* server, struct addrinfo* addressInfo) {
    int serverFd;

    // Attempt to connect to list of returned adresses
    while (addressInfo != NULL)  {
        // Attempt to attach to server socket
        serverFd = socket(addressInfo->ai_family, addressInfo->ai_socktype, 
                addressInfo->ai_protocol);

        server->listenFd = serverFd;

        // Check if attach was successful 
        if (serverFd == -1) {
            continue;
        }

        // Attempt to bind the server to socket
        int serverBindError = server_socket_bind(server, addressInfo);

        // Check if bind was successful
        if (serverBindError) {
            break;
        }

        addressInfo = addressInfo->ai_next;
        close(serverFd);
    }

    if (addressInfo == NULL) {
        // Failed getting socket file descriptor
        server_invalid(server, addressInfo);
    }

    if (listen(serverFd, MAX_CONNECTION_QUEUE) < 0) {
        server_invalid(server, addressInfo);
    }

}

bool server_socket_bind(Server* server, struct addrinfo* addressInfo) {
    int optionValue = 1;

    if (setsockopt(server->listenFd, SOL_SOCKET, SO_REUSEADDR, 
                &optionValue, sizeof(int)) < 0) {
        return false;
    }

    int bindError = bind(server->listenFd, addressInfo->ai_addr, 
            sizeof(struct sockaddr));
    
    if (bindError) {
        return false;
    }

    struct sockaddr_in socketAddress;
    memset(&socketAddress, 0, sizeof(struct sockaddr_in));

    socklen_t socketLength = sizeof(struct sockaddr_in);

    if (getsockname(server->listenFd, (struct sockaddr*) &socketAddress, 
                &socketLength)) {
        return false;
    }

    server->port = ntohs(socketAddress.sin_port);

    return true;
}

void server_invalid(Server* server, struct addrinfo* addressInfo) {
    freeaddrinfo(addressInfo);
    server_destroy(server);            
    error_server_invalid();
}

void server_process_connections(Server* server) {
    int clientFd; 
    struct sockaddr_in clientAddress;
    socklen_t clientAddressSize;
    clientAddressSize = sizeof(struct sockaddr_in);

    while (1) {
        clientFd = accept(server->listenFd, (struct sockaddr*) &clientAddress,
                 &clientAddressSize);

        if (clientFd < 0) {
            continue;
        }

        Client* client = client_create(server, server->clientId++, clientFd);

        map_insert(server->clients, &(client->id), client);

        pthread_create(&(client->threadId), NULL, client_start, client);
        pthread_detach(client->threadId);
    }
}

void server_process_items(Server* server) {
    pthread_t threadId;
    pthread_create(&threadId, NULL, server_process_items_thread, server);
    pthread_detach(threadId);
}

void* server_process_items_thread(void* object) {
    Server* server = (Server*) object;

    while (1) {
        usleep(ITEM_SLEEP_TIME);
        double currentTime = get_time_ms();

        pthread_mutex_lock(server->mutex);

        ListNode* currentItem = server->items->head;
        ListNode* previousItem = NULL;

        // Iterate over item list and check for expiration
        while (currentItem != NULL) {
            Item* item = currentItem->value;
            ListNode* nextItem = currentItem->next;
            
            // Check if item has expired
            if (item->expiry <= currentTime) {
                // Check it item has bidder
                if (item_has_bidder(item)) {
                    request_won_item(item->bidder, item);
                    request_sold_item(item->seller, item);
                } else {
                    request_unsold_item(item->seller, item);
                } 
                
                // Remove the item from the list
                list_remove_node_unsafe(server->items, currentItem, 
                        previousItem);
            }
            
            // Update previous item if current wasn't removed 
            if (currentItem != NULL) {
                previousItem = currentItem;
            }

            currentItem = nextItem; 
        }

        pthread_mutex_unlock(server->mutex);
    }
}

void server_process_stats(Server* server) {
    pthread_t threadId;
    pthread_create(&threadId, NULL, server_process_stats_thread, server);
    pthread_detach(threadId);
}

void* server_process_stats_thread(void* object) {
    Server* server = (Server*) object;

    while (1) {
        int waitError = 0;
        int signal = 0;
        waitError = sigwait(server->signalMask, &signal);

        if (waitError) {
            continue;
        }

        switch (signal) {
            case SIGHUP:
                server_print_stats(server);
                break;
            default:
                break;
        }
    }
}

void server_destroy(void* object) {
    Server* server = (Server*) object;

    map_destroy(server->clients);
    list_destroy(server->items);

    if (server->mutex != NULL) {
        pthread_mutex_destroy(server->mutex);
    }

    if (server->listenFd < 0) {
        close(server->listenFd);
    }
    
    free(server->signalMask);
    free(server);
}

void server_print(Server* server) {
    printf("%d\n", server->port);
    fflush(stdout);
}

void server_print_stats(Server* server) {
    pthread_mutex_lock(server->mutex);
    fprintf(stderr, "Connected clients: %lu\nCompleted clients: %u\n"
            "Active auctions: %lu\nTotal sell requests: %u\n"
            "Successful sell requests: %u\nTotal bid requests: %u\n"
            "Successful bid requests: %u\n", server->clients->size, 
            server->completedClients, server->items->size, server->sellTotal,
            server->sellSuccessful, server->bidTotal, server->bidSuccessful);
    fflush(stderr);
    pthread_mutex_unlock(server->mutex);
}
