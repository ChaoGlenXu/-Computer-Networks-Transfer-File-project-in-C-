/*
** listener.c -- a datagram sockets "server" demo
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define MYPORT "4950"
// the port users will be connecting to
#define MAXBUFLEN 100
// get sockaddr, IPv4 or IPv6:

#define BACKLOG 10
#define BUFFER_SIZE 1100

void *get_in_addr(struct sockaddr *sa)//not used yet
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }   

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char** argv)
{
    char* port_number = argv[1];

    // socket() is used here
    struct addrinfo hints;
    struct addrinfo* res;
    memset(&hints, 0, sizeof hints);// make sure the struct is empty
    hints.ai_family = AF_INET;  /// TCP stream sockets
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    int rv = getaddrinfo(NULL, port_number, &hints, &res);

    int s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);//s is socket

    // use bind() here
    bind(s, res->ai_addr, res->ai_addrlen);

    printf("server: waiting to recvfrom...\n");

    // recvfrom() is used here
    struct sockaddr_storage incoming_addr;
    socklen_t incoming_addr_size = sizeof incoming_addr;

    char buf[BUFFER_SIZE];
    int numbytes = recvfrom(s, buf, BUFFER_SIZE - 1, 0, (struct sockaddr *)&incoming_addr, &incoming_addr_size);



    buf[numbytes] = '\0';

    if(strcmp(buf, "ftp") == 0){
        printf("Ftp service is requested.\n");
        // send information back to the client
        numbytes = sendto(s, "yes", 3, MSG_CONFIRM, (struct sockaddr *)&incoming_addr, incoming_addr_size);
    }else{
        //printf("listener: packet contains \"%s\"\n", buf);
        // send information back to the client
        numbytes = sendto(s, "no", 2, MSG_CONFIRM, (struct sockaddr *)&incoming_addr, incoming_addr_size);
    }
}
