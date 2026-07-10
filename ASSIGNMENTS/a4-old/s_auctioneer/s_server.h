/*
 * ============================================================================
 *
 *       Filename:  server.h
 *
 *    Description:  Header file for server.c
 *
 *        Version:  1.0
 *        Created:  22/10/23 14:41:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4 
 *
 * ============================================================================
 */
#ifndef S_SERVER_H
#define S_SERVER_H

#define MAX_CONNECTION_QUEUE 10
#define ITEM_SLEEP_TIME 100000

#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#include "s_list.h"
#include "s_map.h"

typedef struct Server {
    int listenFd;
    int port;
    size_t clientId;
    size_t maxConnections;
    unsigned int completedClients;
    unsigned int sellTotal;
    unsigned int sellSuccessful;
    unsigned int bidTotal;
    unsigned int bidSuccessful;
    Map* clients;
    List* items;
    pthread_mutex_t* mutex;
    sigset_t* signalMask;
} Server;

void server_start(int argc, char* argv[]);

Server* server_create(int argc, char* argv[]);

void server_open(Server* server, char* port);

struct addrinfo* server_get_address(Server* server, char* port);

void server_socket_connect(Server* server, struct addrinfo* addressInfo);

bool server_socket_bind(Server* server, struct addrinfo* addressInfo);

void server_update_port(Server* server, unsigned int port);

void server_invalid(Server* server, struct addrinfo* addressInfo);

void server_process_connections(Server* server);

void server_process_items(Server* server);

void* server_process_items_thread(void* object);

void server_process_stats(Server* server);

void* server_process_stats_thread(void* object);

void server_destroy(void* object);

void server_print(Server* server);

void server_print_stats(Server* server);

#endif
