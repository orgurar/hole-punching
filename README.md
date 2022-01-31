# UDP Hole Punching

## Description

UDP hole punching is a commonly used technique employed in NAT applications for maintaining UDP packet streams that traverse the NAT.
NAT traversal techniques are typically required for client-to-client networking applications on the Internet involving hosts connected in private networks,
especially in peer-to-peer deployments.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

In order to perform the hole punching technique, you should have 3 entities in a different networks.

The first one should be the third-party server (or a rendezvous server), and it must NOT hide in a network behine a NAT.
If it's public network lays on a NAT, a port forwarding configure can also do the job.

The other two network components are two (or more) different clients, or peers.
The peers <b>can</b> lay behind a NAT, that's the hole point :).

## Running

### Server

The rendezvous server, which is not part of the p2p communication, is located in the `server/bin` folder.

If you wish, you can always compile the source code from `src` folder.

Running the rendezvous server should be as simple as running

```
./server
```

### Client

Running each peer, can be done with just running the bin file from `client/bin` folder with the following command:

```
./client <rendzvous_ip>
```

Again, you can compile yourself if you would like to from client's source couse.

## Authors

- **Or Gur Arie** - Creator and Maintainer - [orgurar](https://gitlab.com/orgurar)

## License

This project is licensed under the MIT license, see the [LICENSE](LICENSE) file for details.
