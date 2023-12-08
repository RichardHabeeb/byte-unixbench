// Unix benchmark socket benchmark inspired core pipe

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "timeit.c"
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

unsigned long iter;

void report(){
    fprintf(stderr, "COUNT|%ld|1|lps\n", iter);
    exit(0);
}

int main(int argc, char *argv[])
{
    printf("A");
    // char buf[512];
    int recv_server = socket(AF_INET, SOCK_STREAM, 0);
    printf("a");
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));

    wake_me(10, report);

    printf("b");

    server.sin_family = AF_INET;
    server.sin_port = htons(4445);
    server.sin_addr.s_addr = INADDR_ANY;

     printf("c");

    if(bind(recv_server, (struct sockaddr *) &server, sizeof(server)) < 0){
        perror("Error on binding");
        exit(1);
    }

    if(listen(recv_server, 1) < 0){
        perror("Error on listen");
        exit(1);
    }

    printf("lit");

    int send_sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(4445);
    if(inet_aton("127.0.0.1", &server.sin_addr) == 0){
        perror("inet_aton error");
        exit(1);
    }

       printf("coc");

    if(connect(send_sock, (struct sockaddr *) &server, sizeof(server)) < 0){
        perror("error on connecting");
    }

    struct sockaddr recv_sockaddr;
    socklen_t recv_len;

    printf("acc");

    int recv_socket = accept(recv_server, &recv_sockaddr, &recv_len);
    if(recv_socket < 0){
        perror("Error on accept");
        exit(1);
    }

    printf("Here");

    char *buf = "potato";
    send(send_sock, buf, 7, 0);
    char bufin[256];
    recv(recv_socket, bufin, 7, 0);
    bufin[7] = '\0';
    printf("recieved %s", bufin);

    iter = 1;
}