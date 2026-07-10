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

#include "sServer.h"

/* Client
 * ---------------
 * Server client data structure to manage state of connections.
 * 
 * Parameters:
 *  server: The server data structure used to manage state.
 *  id: The session id of the client.
 *  clientRead: The open file used to read responses from the client.
 *  clientWrite: The open file used to write requests to the client.
 *  threadId: The id of the thread containing the client process.
 *
 */
typedef struct Client {
    Server* server;
    size_t id;
    FILE* clientRead;
    FILE* clientWrite;
    pthread_t threadId;
} Client;

/* client_create
 * ---------------
 * Initialises and allocates client struct from server.
 * 
 * Parameters:
 *  server: The server data structure used to manage state.
 *  id: The id of the client to create.
 *  fd: The file descriptor of the client.
 *
 * Returns:
 *  Initialised client struct.
 * 
 * Errors:
 *  None
 */
Client* client_create(Server* server, size_t id, int fd);

/* client_start
 * ---------------
 * Starts the server client thread and listens for client server requests 
 * until disconnection.
 * 
 * Parameters:
 *  object: The client data structure to manage state.
 *
 * Returns:
 *  NULL
 * 
 * Errors:
 *  None
 */
void* client_start(void* object);

/* client_disconnect
 * ---------------
 * Disconnects the client from the server, cleans up memory 
 * and sends signal to main thread for disconnection.
 * 
 * Parameters:
 *  client: The client data structure to manage state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 *
 * REF: Used documentation from 'man pthread_mutex_lock'
 * REF: Used documentation from 'man pthread_cond_signal'
 */
void client_disconnect(Client* client);

/* client_from_id
 * ---------------
 * Get the client struct from its ID.
 * 
 * Parameters:
 *  server: The server data structure used to manage state.
 *  id: The id of the client to get.
 *
 * Returns:
 *  The client data structure for the given id.
 *  
 * Errors:
 *  None
 */
Client* client_from_id(Server* server, size_t id);

/* client_compare_map
 * ---------------
 * Checks if two client keys are the same. Used in map.
 * 
 * Parameters:
 *  key1: The key of the first client to compare.
 *  key2: The key of the second client to compare.
 *
 * Returns:
 *  True if the clients have the same key, false otherwise.
 * 
 * Errors:
 *  None
 */
bool client_compare_map(void* key1, void* key2);

/* client_print_map
 * ---------------
 * Prints client key and value to the terminal.
 * 
 * Parameters:
 *  key: The id key of the client.
 *  value: The client data structure.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void client_print_map(void* key, void* value);

/* client_print
 * ---------------
 * Print the client to the terminal.
 * 
 * Parameters:
 *  object: The client data structure to print.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void client_print(void* object);

/* client_destroy_map
 * ---------------
 * Destroys and frees memory of client map object.
 * 
 * Parameters:
 *  key: The map key to destory.
 *  value: The map value to destory.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void client_destroy_map(void* key, void* value);

/* client_destroy
 * ---------------
 * Destroys and frees memory of client data structure.
 * 
 * Parameters:
 *  object: The client to destroy.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void client_destroy(void* object);

#endif
