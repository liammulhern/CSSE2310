/*
 * ============================================================================
 *
 *       Filename:  client.h
 *
 *    Description:  Header file for client.c
 *
 *        Version:  1.0
 *        Created:  15/10/23 11:32:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4
 *
 * ============================================================================
 */
#ifndef C_CLIENT_H
#define C_CLIENT_H

#define SERVER_IP "localhost"

#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#include "c_arguments.h"

typedef struct Client {
    FILE* serverRead;
    FILE* serverWrite;
    char* port;
    unsigned int listedItems;
    unsigned int biddingItems;
    pthread_mutex_t* mutex;
} Client;

void client_start(int argc, char* argv[]);

Client* client_create(int argc, char* argv[]);

void client_create_sigpipe_signal();

void client_sigpipe_signal_handler(int signal);

void client_server_connect(Client* client);

struct addrinfo* client_get_address(Client* client);

void client_socket_connect(Client* client, struct addrinfo* addressInfo);

void client_port_invalid(Client* client);

void client_request_thread_create(Client* client);

void* client_request(void* arg);

void client_response(Client* client);

void client_server_close(Client* client);

void client_close(Client* client);

void client_quit(Client* client);

bool client_is_in_progress(Client* client);

void client_print(Client* client);

void client_print_address(struct addrinfo* addressInfo);

void client_destroy(void* object);

#endif
