#include "framework.h"
#include "peer.h"

int main(void)
{
    // server socket and address
    // remember that the server should run in a different network
    // with UDP endpoint previously known and directly accessible by all peers.
    // in other words, it must NOT hide behine a NAT
    // or should have port forwarding preconfigured on a known port.
    struct sockaddr_in servaddr;
    int sockfd;

    // local list of connected client, up to 5
    struct peer clients[MAXCLIENTS];
    int i, j, n = 0;

    // initiate server address to our local endpoint
    // and Filling its information
    memset((char *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // create the main server socket
    // AF_INET sets the IPv4 Internet protocols
    // SOCK_DGRAM means UDP protocol
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == SOCKERR)
        term("ERR: socket creation failed");
    else
        printf("[+] server socket connected seccessfully\n");

    // When a socket is created with socket(), it exists in a name
    // space(address family) but has no address assigned to it.
    // bind() assigns the address specified by addr to the socket referred to
    // by the file descriptor sockfd.
    if (bind(sockfd, (struct sockaddr *)(&servaddr), sizeof(servaddr)) == SOCKERR)
        term("ERR: socket bind failed");
    else
        printf("[+] socket binded\n");

    // server is listening, now we start to
    // accept client hole punch datagrams
    printf("[+] listening to incoming datagrams on port %d\n", PORT);
    while (1)
    {
        // temp current connecting client info
        struct sockaddr_in clientaddr;
        int addrsize = sizeof(clientaddr);
        char buf[BUFSIZE];

        // udp message received from a new client
        // insert the client UDP public endpoint into clientaddr struct
        if (recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *)(&clientaddr), &addrsize) == SOCKERR)
            term("ERR: could not recieve from socket");

        // the client's NAT has created the entry
        printf("[?] %s:%d has just punched a hole!\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        if (n == MAXCLIENTS)
            term("ERR: server peers list is full");

        // add the new client to our local UDP endpoints list
        clients[n].host = clientaddr.sin_addr.s_addr;
        clients[n].port = clientaddr.sin_port;
        n++;

        // then notify all the connected peers
        for (i = 0; i < n; i++)
        {
            // use clientaddr as a temp client address object
            clientaddr.sin_addr.s_addr = clients[i].host;
            clientaddr.sin_port = clients[i].port;

            // send a datagram to each client in our list
            printf("[?] notifing %s:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
            for (j = 0; j < n; j++)
            {
                // The payload is the client's public UDP endpoint, clients[j]
                if (sendto(sockfd, &clients[j], 6, 0, (struct sockaddr *)(&clientaddr), addrsize) == SOCKERR)
                    term("socket send failed");
            }
        }

        // total peers conected
        printf("%d peers are currently connected\n", n);
    }

    close(sockfd);
    return 0;
}