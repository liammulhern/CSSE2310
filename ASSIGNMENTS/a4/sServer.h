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

#define MAX_CONNECTION_QUEUE 0
#define ITEM_SLEEP_TIME 100000

#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#include "sList.h"
#include "sMap.h"

/* Server
 * ---------------
 * Server data structure that manages state.
 * 
 * Parameters:
 *  listenFd: The file descriptor that the server listens for
 *      connections on.
 *  port: The port that the server is bound to.
 *  clientId: The session ID of the next client to join.
 *  maxConnections: The maximum number of connections to the server.
 *  completedClients: The number of complete client connections.
 *  sellTotal: The total number of sell requests made.
 *  sellSuccessful: The total number of valid sell requests made.
 *  bidTotal: The total number of bid requests made.
 *  bidSuccessful: The total number of valid bid requests made.
 *  clients: Map of clients currently connected to server.
 *  items: List of items currently auctioned.
 *  mutex: Thread mutex for shared data structures.
 *  connection: Thread condition wait function.
 *  signalMask: The signal mask used by threads.
 */
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
    pthread_cond_t* connection;
    sigset_t* signalMask;
} Server;

/* server_start
 * ---------------
 * Starts and creates the server, attempting to open
 * server.
 * 
 * Parameters:
 *  argc: The argument count passed to the main function from the console.
 *  argv: The argument array passed to the main function from the console.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  If server cannot be started, client exits via error_server_invalid()
 */
void server_start(int argc, char* argv[]);

/* server_create
 * ---------------
 * Create the server struct and set default values.
 * 
 * Parameters:
 *  argc: The argument count passed to the main function from the console.
 *  argv: The argument array passed to the main function from the console.
 *
 * Returns:
 *  Initialised and allocated server struct.
 * 
 * Errors:
 *  If arguments cannot be parsed exits via error_invalid_arguments()
 */
Server* server_create(int argc, char* argv[]);

/* server_set_defaults
 * ---------------
 * Set the initial defaults for a server.
 * 
 * Parameters:
 *  server: Server data structure that manages state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void server_set_defaults(Server* server);

/* server_open 
 * ---------------
 * Attempts to open the server of port.
 * 
 * Parameters:
 *  server: Server data structure that manages state.
 *  port: The port to bind the server to. If 0 bind to ephemeral.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  If server cannot be opened, exits via error_server_invalid()
 */
void server_open(Server* server, char* port);

/* client_get_address
 * ---------------
 * Attempts to get the server address from the server information.
 * Returns the address info if successful.
 * 
 * Parameters:
 *  server: Server data structure that manages state.
 *  port: The port to bind the server to. If 0 bind to ephemeral.
 *
 * Returns:
 *  The server addressInfo if successful.
 * 
 * Errors:
 *  If server address info cannot be found, exits via error_port_invalid()
 */
struct addrinfo* server_get_address(Server* server, char* port);

/* server_socket_connect 
 * ---------------
 * Attempts to connect the server to socket.
 * 
 * Parameters:
 *  server: Server data structure that manages state.
 *  addressInfo: The addressInfo of the server that the client is attempting 
 *      to connect to.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  If socket cannot be connected to, exits via error_server_invalid()
 */
void server_socket_connect(Server* server, struct addrinfo* addressInfo);

/* server_socket_connect 
 * ---------------
 * Attempts to bind the server to socket, returns true if successful.
 * 
 * Parameters:
 *  server: Server data structure that manages state.
 *  addressInfo: The addressInfo of the server that the client is attempting 
 *      to connect to.
 *
 * Returns:
 *  True if bind is successful, false otherwise.
 * 
 * Errors:
 *  If socket cannot be connected to, exits via error_server_invalid()
 */
bool server_socket_bind(Server* server, struct addrinfo* addressInfo);

/* server_invalid
 * ---------------
 * Server start was invalid, destroys and frees memory before exit.
 * 
 * Parameters:
 *  server: Server data structure that manages state.
 *  addressInfo: The addressInfo of the server that the client is attempting 
 *      to connect to.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits via error_server_invalid()
 */
void server_invalid(Server* server, struct addrinfo* addressInfo);

/* server_process_connections
 * ---------------
 * Listen and accept connections from clients, starting new Thread
 * for each connection.
 * 
 * Parameters:
 *  server: Server data structure that manages state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 *
 * REF: Uses documentation from 'man pthread_cond_wait'
 */
void server_process_connections(Server* server);

/* server_process_items
 * ---------------
 * Start item process thread to check for auction expiry.
 * 
 * Parameters:
 *  server: Server data structure that manages state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void server_process_items(Server* server);

/* server_process_items_thread
 * ---------------
 * Manages auction item expiry checking all items listed 
 * every 100ms.
 * 
 * Parameters:
 *  object: Server data structure that manages state.
 *
 * Returns:
 *  NULL
 * 
 * Errors:
 *  None
 */
void* server_process_items_thread(void* object);

/* server_process_items_removal
 * ---------------
 * Thread safe removal of items from server item list.
 * 
 * Parameters:
 *  server: Server data structure that manages state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void server_process_items_removal(Server* server);

/* server_process_stats
 * ---------------
 * Start statistics process thread and wait for SIGHUP.
 * 
 * Parameters:
 *  server: Server data structure that manages state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void server_process_stats(Server* server);

/* server_process_stats_thread
 * ---------------
 * Wait for SIGHUP signal and print current server runtime 
 * statistics when recieved.
 * 
 * Parameters:
 *  object: Server data structure that manages state.
 *
 * Returns:
 *  NULL 
 * 
 * Errors:
 *  None
 */
void* server_process_stats_thread(void* object);

/* server_destroy
 * ---------------
 * Destory the server data structure, closing all clients and
 * freeing memory.
 * 
 * Parameters:
 *  object: Server data structure that manages state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void server_destroy(void* object);

/* server_print
 * ---------------
 * Print the server data structure, including port to the terminal
 * 
 * Parameters:
 *  server: Server data structure that manages state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void server_print(Server* server);

/* server_print_stats
 * ---------------
 * Print the server stats to the terminal.
 * 
 * Parameters:
 *  server: Server data structure that manages state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void server_print_stats(Server* server);

#endif
