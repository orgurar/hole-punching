#include "framework.h"
#include "peer.h"

int main(void)
{
    // client socket
    // we will use this socket to communicate both with
    // the 3td party server and the other peers.
    struct sockaddr_in clientaddr;
    int sockfd;
    struct sockaddr_in servaddr;
    int addrsize = sizeof(servaddr);

    // local list of connected clients, up to 5
    struct peer clients[MAXCLIENTS];
    int i, j, n = 0;

    // store server information inside a peer struct.
    // this will allow us to easy identify server datagrams
    struct peer server;

    // initiate our address and local endpoint
    memset((char *)&clientaddr, 0, sizeof(clientaddr));
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_port = htons(0);
    clientaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // now save server address both in peer and sockaddr structs
    memset((char *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVPORT);
    if (inet_aton(SERVADDR, &servaddr.sin_addr) == 0)
        term("ERR: aton server ip");

    // store server information in peer struct as well
    server.host = servaddr.sin_addr.s_addr;
    server.port = servaddr.sin_port;

    // now we have all set.
    // we can send a simple datagram to our server
    // and let it know our public endpoint.
    // this request will also punch a hole in out NAT,
    // meaning that a new entry will be added mapping a random port
    // to our local port. the server will let us know what the random port is.
    if (sendto(sockfd, "punch", 5, 0, (struct sockaddr *)(&servaddr), addrsize) == SOCKERR)
        term("ERR: sendto failed");

    // now our NAT should have a new entry between our host and the server.
    // when another peer will try to reach on us, it will pass the packets to
    // our local enrty.
    while (1)
    {
        // temp current connecting client info
        struct sockaddr_in clientaddr;
        int addrsize = sizeof(clientaddr);

        // note that we are getting the information as a peer
        struct peer buf;

        // recive a new message. note that we are using the same socket for both
        // server and peers connection, so in order to determine we have to check
        // manualy the sender's endpoint
        if (recvfrom(sockfd, &buf, sizeof(buf), 0, (struct sockaddr *)(&clientaddr), &addrsize) == SOCKERR)
            term("ERR: recvfrom failed");

        printf("revieved a new packet from %s:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        // identify if the server came from the message
        if (server.host == clientaddr.sin_addr.s_addr && server.port == (short)(clientaddr.sin_port))
        {
            // the message came from the server
            // messaged like this is an update packets contains
            // every client's public and open endpoint.
            int peerexists = 0;
            for (i = 0; i < n && !peerexists; i++)
                if (clients[i].host == buf.host && clients[i].port == buf.port)
                    peerexists = 1; // found the peer

            // now, if it doesn't exist in our local list, we shoul add it
            if (!peerexists)
            {
                clients[n].host == buf.host;
                clients[n].port == buf.port;
                n++;
            }

            // peer added
            clientaddr.sin_addr.s_addr = buf.host;
            clientaddr.sin_port = buf.port;
            printf("[?] added peer %s:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
            printf("Now we have %d peers\n", n);

            // now we send datagrams to all peer and can commuicate via out p2p model
            for (i = 0; i < n; i++)
            {
                clientaddr.sin_addr.s_addr = clients[i].host;
                clientaddr.sin_port = clients[i].port;

                // Once again, the payload is irrelevant. Feel free to send your VoIP
                // data in here.
                if (sendto(sockfd, "p2p", 3, 0, (struct sockaddr *)(&clientaddr), addrsize) == SOCKERR)
                    diep("ERR: cannot send");
            }
        }
        else
        {
            // the message came from another peer
            for (i = 0; i < n; i++)
            {
                // Identify which peer it came from
                if (clients[i].host == buf.host && clients[i].port == (short)(buf.port))
                {
                    clientaddr.sin_addr.s_addr = buf.host;
                    clientaddr.sin_port = buf.port;
                    // And do something useful with the received payload
                    printf("[!] recieved from peer %s:%d!\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
                    break;
                }
            }
        }
    }

    close(sockfd);
    return 0;
}