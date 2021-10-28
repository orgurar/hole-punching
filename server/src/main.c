#include "framework.h"

// terminate function in case something goes wrong
void term(char *e)
{
    perror(e);
    exit(1);
}

int main(void)
{
    // server socket and address
    int sockfd;
    struct sockaddr_in servaddr;
    char buf[BUFSIZE];

    // current connecting client address
    struct sockaddr_in clientaddr;
    int slen = sizeof(clientaddr);

    // list of connected client, up to 3
    struct peer clients[MAXCLIENTS];
    int n = 0;

    // create the main server socket
    // AF_INET sets the IPv4 Internet protocols
    // SOCK_DGRAM means UDP protocol
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == SOCKERR)
        term("socket creation failed");

    // initiate servaddr to our local endpoint.
    // remember that the server should run in a different network
    // with UDP endpoint previously known and directly accessible
    // by all clients. The server cannot be behind a NAT!
    memset((char *)&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // When a socket is created with socket(2), it exists in a name
    // space(address family) but has no address assigned to it
    //bind() assigns the address specified by addr to the socket referred to
    // by the file descriptor sockfd.
    if (bind(sockfd, (struct sockaddr *)(&servaddr), sizeof(servaddr)) == SOCKERR)
        term("bind failed");

    // start listening to client hole punch datagrams
    while (1)
    {
        // udp message received from a new client
        if (recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *)(&clientaddr), &slen) == SOCKERR)
            diep("socket recive failed");
        // The client's public UDP endpoint data is now in si_other.
        // the server doesn't care about the datagram's payload,
        // the client's NAT has created the entry

        printf("%s:%d Just Punched a Hole!\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        // add the new client to our local UDP endpoints list
        clients[n].host = clientaddr.sin_addr.s_addr;
        clients[n].port = clientaddr.sin_port;
        n++;
    }

    return 0;
}