/*
 * ============================================================================
 *
 *       Filename:  client.c
 *
 *    Description:  Handles server client thread responsibilities.
 *
 *        Version:  1.0
 *        Created:  23/10/23 11:19:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4
 *
 * ============================================================================
 */
#include <stdlib.h>

#include "s_client.h"
#include "s_response.h"
#include "s_item.h"

Client* client_create(Server* server, size_t id, int fd) {
    Client* client = (Client*) malloc(sizeof(Client));

    client->id = id;
    client->server = server;
    client->clientRead = fdopen(fd, "r");
    client->clientWrite = fdopen(fd, "w");

    return client;
}

void* client_start(void* object) {
    Client* client = (Client*) object;

    response_start(client);

    return NULL;
}

void client_disconnect(Client* client) {
    pthread_mutex_lock(client->server->mutex);
    client->server->completedClients++;
    pthread_mutex_unlock(client->server->mutex);

    map_remove(client->server->clients, &(client->id));
}

bool client_compare_map(void* object1, void* object2) {
    size_t clientId1 = *(size_t*) object1;
    size_t clientId2 = *(size_t*) object2;
    
    return clientId1 == clientId2;
}

void client_print_map(void* key, void* value) {
    size_t clientId = *(size_t*) key;
    printf("%ld: [", clientId);
    client_print(value);
    printf("]");
}

void client_print(void* object) {
    Client* client = (Client*) object;

    printf("ID: %ld, Read/Write: %d/%d, Thread: %lu", client->id, 
            fileno(client->clientRead), fileno(client->clientRead), 
            client->threadId);

    fflush(stdout);
}

void client_destroy_map(void* key, void* value) {
    client_destroy(value);
}

void client_destroy(void* object) {
    Client* client = (Client*) object;
    
    if (client->clientRead != NULL) {
        fclose(client->clientRead);
        client->clientRead = NULL;
    }

    if (client->clientWrite != NULL) {
        fclose(client->clientWrite);
        client->clientWrite = NULL;
    }

    free(client);
}
