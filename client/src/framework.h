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

#define SERVADDR "213.57.241.210"
#define SERVPORT 8080
#define BUFSIZE 512

#define MAXCLIENTS 5
#define SOCKERR -1

// terminate program in case something goes wrong
void term(char *e)
{
    perror(e);
    exit(1);
}