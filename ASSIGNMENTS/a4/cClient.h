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

#include "cArguments.h"

/* Client
 * ---------------
 * Struct for handling aucion client state.
 * 
 * Parameters:
 *  serverRead: Open file used to read server response.
 *  severWrite: Open file used to write server requests.
 *  port: The port that the client is connected to.
 *  listedItems: The number of items the client is currently selling.
 *  biddingItems: The number of item the client is bidding on.
 *  mutex: The mutex used for thread safe sharing of data structures.
 */
typedef struct Client {
    FILE* serverRead;
    FILE* serverWrite;
    char* port;
    unsigned int listedItems;
    unsigned int biddingItems;
    pthread_mutex_t* mutex;
} Client;

/* client_start
 * ---------------
 * Starts and creates the client, attempting to connect to the server.
 * 
 * Parameters:
 *  argc: The argument count passed to the main function from the console.
 *  argv: The argument array passed to the main function from the console.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  If port cannot be connected, client exits via error_port_invalid()
 */
void client_start(int argc, char* argv[]);

/* client_create
 * ---------------
 * Create the client struct and set default values.
 * 
 * Parameters:
 *  argc: The argument count passed to the main function from the console.
 *  argv: The argument array passed to the main function from the console.
 *
 * Returns:
 *  Initialised and allocated client struct.
 * 
 * Errors:
 *  If arguments cannot be parsed exits via error_invalid_arguments()
 */
Client* client_create(int argc, char* argv[]);

/* client_create_sigpipe_signal
 * ---------------
 * Initialises the SIGPIPE signal handler.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void client_create_sigpipe_signal();

/* client_sigpipe_signal_handler
 * ---------------
 * Handler method for SIGPIPE signal.
 * 
 * Parameters:
 *  signal: The signal recieved by the handler.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void client_sigpipe_signal_handler(int signal);

/* client_server_connect
 * ---------------
 * Attempts to connect the client to the server.
 * 
 * Parameters:
 *  client: The client information used to connect to the server 
 *      and manage state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  If server cannot be connected to, exits via error_port_invalid()
 */
void client_server_connect(Client* client);

/* client_get_address
 * ---------------
 * Attempts to get the server address from the client information.
 * Returns the address info if successful.
 * 
 * Parameters:
 *  client: The client information used to connect to the server 
 *      and manage state.
 *
 * Returns:
 *  The server addressInfo if successful.
 * 
 * Errors:
 *  If server address info cannot be found, exits via error_port_invalid()
 */
struct addrinfo* client_get_address(Client* client);

/* client_socket_connect
 * ---------------
 * Attempts to connect to the server socket using the addressInfo.
 * 
 * Parameters:
 *  client: The client information used to connect to the server 
 *      and manage state.
 *  addressInfo: The addressInfo of the server that the client is attempting 
 *      to connect to.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  If socket cannot be connected to exits via error_port_invalid()
 */
void client_socket_connect(Client* client, struct addrinfo* addressInfo);

/* client_port_invalid
 * ---------------
 * Cleans up client data if port cannot be connected to.
 * 
 * Parameters:
 *  client: The client information used to connect to the server 
 *      and manage state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits via error_port_invalid()
 */
void client_port_invalid(Client* client);

/* client_request_thread_create
 * ---------------
 * Creates thread for writing client requests to the server.
 * 
 * Parameters:
 *  client: The client information used to connect to the server 
 *      and manage state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void client_request_thread_create(Client* client);

/* client_request
 * ---------------
 * Starts client request handler until closed. 
 * 
 * Parameters:
 *  object: The client information used to connect to the server 
 *      and manage state.
 *
 * Returns:
 *  NULL
 * 
 * Errors:
 *  Closes client if request fails.
 */
void* client_request(void* object);

/* client_response
 * ---------------
 * Starts client reponse handler until closed.
 * 
 * Parameters:
 *  client: The client information used to connect to the server 
 *      and manage state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Closes client if response fails.
 */
void client_response(Client* client);

/* client_server_close
 * ---------------
 * Frees client data when server is closed. 
 * 
 * Parameters:
 *  client: The client information used to connect to the server 
 *      and manage state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits via error_server_closed()
 */
void client_server_close(Client* client);

/* client_close
 * ---------------
 * Close the client and checks if auction is in progress.
 * 
 * Parameters:
 *  client: The client information used to connect to the server 
 *      and manage state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  Exits via error_quit if no auctions are in program,
 *  otherwise error_client_closed().
 */
void client_close(Client* client);

/* client_quit
 * ---------------
 * Attempts to close the client if no auctions are in progress.
 * 
 * Parameters:
 *  client: The client information used to connect to the server 
 *      and manage state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  If auction is not in progress exits via error_quit().
 */
void client_quit(Client* client);

/* client_is_in_progress
 * ---------------
 * Checks if a client is currently bidding or selling items in an auctions.
 * 
 * Parameters:
 *  client: The client information used to connect to the server 
 *      and manage state.
 *
 * Returns:
 *  True if client is currently bidding or selling, false otherwise.
 * 
 * Errors:
 *  None
 */
bool client_is_in_progress(Client* client);

/* client_print
 * ---------------
 * Prints the client information to the terminal.
 * 
 * Parameters:
 *  client: The client information used to connect to the server 
 *      and manage state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void client_print(Client* client);

/* client_print_address
 * ---------------
 * Prints the client address information to the terminal.
 * 
 * Parameters:
 *  addressInfo: The addressInfo used to connect to the server.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void client_print_address(struct addrinfo* addressInfo);

/* client_destroy
 * ---------------
 * Destroys the client struct and frees allocated memory.
 * 
 * Parameters:
 *  object: The client information used to connect to the server 
 *      and manage state.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void client_destroy(void* object);

#endif
