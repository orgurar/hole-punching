/**
 * 
 * This file provides a C based UDP server framework
 * 
*/

// c standart headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// socket header files
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFSIZE 512
#define SOCKERR -1

#define MAXCLIENTS 3

// a small struct used to hold a UDP peer connected
struct peer
{
    int host;
    short port;
};
