/*
 * ============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Entry point of auctioneer server.
 *
 *        Version:  1.0
 *        Created:  22/10/23 11:48:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A4
 *
 * ============================================================================
 */
#include <stdlib.h>

#include "sServer.h"

int main(int argc, char* argv[]) {
    server_start(argc, argv);
}
