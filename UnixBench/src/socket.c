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

int recv_server, recv_socket, send_sock;
int port;

void report(){
    fprintf(stderr, "COUNT|%ld|1|lps\n", iter);
    close(recv_socket);
    shutdown(recv_server, 2);
    close(recv_server);
    close(send_sock);
    exit(0);
}

int main(argc, argv)
int argc;
char *argv[];
{
    int duration;

    if(argc != 3){
        fprintf(stderr, "Usage: %s duration starting_port\n", argv[0]);
        exit(1);
    }

    duration = atoi(argv[1]);
    port = atoi(argv[2]);

    recv_server = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    while(bind(recv_server, (struct sockaddr *) &server, sizeof(server)) < 0){
        if(errno = EADDRINUSE)
            {server.sin_port = htons(++port);}
        else{
            perror("Error on binding");
            exit(1);
        }
    }

    if(listen(recv_server, 1) < 0){
        perror("Error on listen");
        exit(1);
    }

    send_sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if(inet_aton("127.0.0.1", &server.sin_addr) == 0){
        perror("inet_aton error");
        exit(1);
    }

    if(connect(send_sock, (struct sockaddr *) &server, sizeof(server)) < 0){
        perror("error on connecting");
    }

    struct sockaddr recv_sockaddr;
    socklen_t recv_len;

    recv_socket = accept(recv_server, &recv_sockaddr, &recv_len);
    if(recv_socket < 0){
        perror("Error on accept");
        exit(1);
    }

    char buf[512];

    wake_me(duration, report);
    iter = 0;

    while (1){
        if(send(send_sock, buf, sizeof(buf), 0) != sizeof(buf)){
            if((errno != EINTR) && (errno != 0))
                fprintf(stderr,"send failed, error %d\n", errno);
        }
        if(recv(recv_socket, buf, sizeof(buf), 0) != sizeof(buf)){
            if((errno != EINTR) && (errno != 0))
                fprintf(stderr,"recv failed, error %d\n", errno);
        }
        iter++;
    }
}