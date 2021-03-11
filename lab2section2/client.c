/*
** talker.c -- a datagram "client" demo
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


#include <sys/time.h>  // this library is for calculate time for section 2

#define SERVERPORT "4950"
// the port users will be connecting to
int main(int argc, char *argv[])
{
    int sockfd;
    struct timeval start;
    struct timeval end;
    double RRT;
    

    char* destAddr = argv[1];
    char* portNum = argv[2];

    // ask for user input
    printf("Enter command:\n");

    char command[100];
    char fileSrc[100];

    scanf("%s", command);
    scanf("%s", fileSrc);


    // check if the file exists
    if( access( fileSrc, F_OK ) != -1 ) {
        printf(" File exists.\n");
    } else {
        printf(" File not found.\n");
        return (EXIT_FAILURE);
    }


    // socket() is used here
    int s = socket(AF_INET, SOCK_DGRAM, 0);

    // sendto() is used here
    struct addrinfo hints;
    struct addrinfo* res;
    memset(&hints, 0, sizeof hints);// make sure the struct is empty
    hints.ai_family = AF_INET;  // TCP stream sockets
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    int rv = getaddrinfo(destAddr, portNum, &hints, &res);

    gettimeofday(&start, NULL); // start clocking now because right before the sendto function is called.   //section 2      
   
    int numbytes = sendto(s, "ftp to the server", 3, MSG_CONFIRM, res->ai_addr, res->ai_addrlen);
    if (numbytes == -1){
        printf("Failed to send to server.\n");
        return (EXIT_FAILURE);
    }else { printf("Successfully sent to server.\n");}




    // recvfrom() is used here
    struct sockaddr_storage incoming_addr;
    socklen_t incoming_addr_size = sizeof incoming_addr;

    char buffer[100];
    recvfrom(s, buffer, 99, 0, (struct sockaddr *)&incoming_addr, &incoming_addr_size);

    if(strcmp(buffer, "yes") == 0){
        printf("A file transfer can start now\n");
    }

//the lab 2 section 2 below    
    gettimeofday(&end, NULL); // stop clocking here //section 2     
    long micros = end.tv_usec - start.tv_usec;
    printf("lab2 section2's round-trip time -> RRT : %lu us\n", micros);
//after see the RRT, the round-trip time is around 360 us
//the lab 2 section 2 above    



}
