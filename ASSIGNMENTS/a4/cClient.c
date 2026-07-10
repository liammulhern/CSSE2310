/*
 * ============================================================================
 *
 *       Filename:  client.c
 *
 *    Description:  Client handler
 *
 *        Version:  1.0
 *        Created:  15/10/23 11:14:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4
 *
 * ============================================================================
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <csse2310a3.h>
#include <signal.h>

#include "cClient.h"
#include "cError.h"
#include "cRequest.h"
#include "cResponse.h"

void client_start(int argc, char* argv[]) {
    Client* client = client_create(argc, argv);
    
    client_server_connect(client);

    client_request_thread_create(client);
    client_response(client);

    client_destroy(client);
}

Client* client_create(int argc, char* argv[]) {
    Client* client = (Client*) malloc(sizeof(Client));

    // Get command line arguments
    Arguments* arguments = arguments_get(argc, argv);
    
    // Create mutex fo all threads to share
    pthread_mutex_t* mutex = (pthread_mutex_t*) malloc(sizeof(
            pthread_mutex_t));
    pthread_mutex_init(mutex, NULL);

    // Set default values
    client->serverRead = NULL;
    client->serverWrite = NULL;
    client->listedItems = 0;
    client->biddingItems = 0;
    client->port = arguments->portNumber;
    client->mutex = mutex;

    arguments_destroy(arguments);
    client_create_sigpipe_signal();

    return client;
}

void client_create_sigpipe_signal() {
    struct sigaction saInterupt;
    memset(&saInterupt, 0, sizeof(saInterupt));

    saInterupt.sa_handler = client_sigpipe_signal_handler;
    saInterupt.sa_flags = 0;

    sigaction(SIGPIPE, &saInterupt, 0);
}

void client_sigpipe_signal_handler(int signal) {
    return;
}

void client_server_connect(Client* client) {
    struct addrinfo* addressInfo;

    addressInfo = client_get_address(client);
    client_socket_connect(client, addressInfo);
}

struct addrinfo* client_get_address(Client* client) {
    struct addrinfo* addressInfo;
    struct addrinfo addressHints;
    memset(&addressHints, 0, sizeof(addressHints));

    addressHints.ai_family = AF_INET;
    addressHints.ai_socktype = SOCK_STREAM;
    addressHints.ai_flags = 0;
    addressHints.ai_protocol = 0;

    int addressError = getaddrinfo(SERVER_IP, client->port, &addressHints, 
            &addressInfo);

    if (addressError != 0) {
        // Failed getting address info
        client_port_invalid(client);
    }

    return addressInfo;
}

void client_socket_connect(Client* client, struct addrinfo* addressInfo) {
    int serverFd;

    // Attempt to connect to list of returned adresses
    while (addressInfo != NULL)  {
        // Attempt to attach to server socket
        serverFd = socket(addressInfo->ai_family, addressInfo->ai_socktype, 
                addressInfo->ai_protocol);

        // Check if attach was successful 
        if (serverFd == -1) {
            continue;
        }

        // Attempt to connect to the server
        int serverConnectionError = connect(serverFd, addressInfo->ai_addr, 
                addressInfo->ai_addrlen);

        // Check if connection was successful
        if (serverConnectionError != -1) {
            break;
        }

        addressInfo = addressInfo->ai_next;
        close(serverFd);
    }

    if (addressInfo == NULL) {
        // Failed getting socket file descriptor
        client_port_invalid(client);
    }

    freeaddrinfo(addressInfo);

    client->serverRead = fdopen(serverFd, "r");
    client->serverWrite = fdopen(serverFd, "w");
}

void client_port_invalid(Client* client) {
    char* port = strdup(client->port);

    client_destroy(client);            
    error_port_invalid(port);
}

void client_request_thread_create(Client* client) {
    pthread_t threadId;
    pthread_create(&threadId, NULL, client_request, client);
    pthread_detach(threadId);
}

void* client_request(void* object) {
    Client* client = (Client*) object;
    
    request_start(client);
    client_close(client);

    return NULL;
}

void client_response(Client* client) {
    response_start(client);
    client_server_close(client);
}

void client_server_close(Client* client) {
    client_destroy(client);
    error_server_closed();
}

void client_close(Client* client) {
    if (client_is_in_progress(client)) {
        error_client_closed();
    } else {
        error_quit();
    }
}

void client_quit(Client* client) {
    if (client_is_in_progress(client)) {
        printf("Auction in progress - unable to exit yet\n");
        fflush(stdout);
    } else {
        error_quit();
    }
}

bool client_is_in_progress(Client* client) {
    pthread_mutex_lock(client->mutex);
    bool inProgress = client->listedItems > 0 || client->biddingItems > 0;
    pthread_mutex_unlock(client->mutex);

    return inProgress;
}

void client_print(Client* client) {
    printf("Port: %s\n", client->port);
    printf("Server Write File Descriptor: %d\n", fileno(client->serverWrite));
    printf("Server Read File Descriptor: %d\n", fileno(client->serverRead));
}

void client_print_address(struct addrinfo* addressInfo) {
    printf("======= Address Info =======\n");
    printf("Address Flags: %d\n", addressInfo->ai_flags);
    printf("Address Family: %d\n", addressInfo->ai_family);
    printf("Address Socket Type: %d\n", addressInfo->ai_socktype);
    printf("Address Protocol: %d\n", addressInfo->ai_protocol);
    printf("Address Length: %d\n", addressInfo->ai_addrlen);
    printf("Address Canon Name: %s\n", addressInfo->ai_canonname);
    printf("Address Next: %p\n", addressInfo->ai_canonname);
    
    struct sockaddr* socketAddress = addressInfo->ai_addr;

    printf("======= Socket Info =======\n");
    printf("Socekt Family: %u\n", socketAddress->sa_family);
    printf("Socket Data: %s\n", socketAddress->sa_data);
}

void client_destroy(void* object) {
    Client* client = (Client*) object;

    if (client->mutex != NULL) {
        pthread_mutex_destroy(client->mutex);
    }

    if (client->serverRead != NULL) {
        fclose(client->serverRead);
    }

    if (client->serverWrite != NULL) {
        fclose(client->serverWrite);
    }

    free(client);
}
