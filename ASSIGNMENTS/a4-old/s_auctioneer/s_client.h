/*
 * ============================================================================
 *
 *       Filename:  client.h
 *
 *    Description:  Header file for client.c
 *
 *        Version:  1.0
 *        Created:  23/10/23 11:19:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4 
 *
 * ============================================================================
 */
#ifndef S_CLIENT_H
#define S_CLIENT_H

#include <stdio.h>

#include "s_server.h"

typedef struct Client {
    Server* server;
    size_t id;
    FILE* clientRead;
    FILE* clientWrite;
    pthread_t threadId;
} Client;

Client* client_create(Server* server, size_t id, int fd);

void* client_start(void* object);

void client_disconnect(Client* client);

void client_response(Client* client);

bool client_compare_map(void* object1, void* object2);

void client_print_map(void* key, void* value);

void client_print(void* object);

void client_destroy_map(void* key, void* value);

void client_destroy(void* object);

#endif
