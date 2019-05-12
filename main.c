#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 10001
#define MAX_CONNECTION 1

int hErr(int code, const char* msg) { //handle error
    if(code<=-1) {
        fprintf(stderr, "[ERR] %s %i\n", msg, code);
        exit(1);
    } else {
        printf("[OK] %s\n", msg);
    }
    return code;
}

void client() {
    const char *host = "localhost";
    unsigned short port = PORT;
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host, &addr.sin_addr);
    const int sock = hErr(socket(AF_INET, SOCK_STREAM, 0), "SOCKET");
    hErr(connect(sock, (struct sockaddr*)&addr, sizeof(addr)), "CONNECT");
    char bufor[16] = "r";
    hErr(send(sock, bufor, 1, 0)-1, "SEND");
    do {
        hErr(recv(sock, bufor, sizeof(bufor), 0)-1, "RECV");
        printf("%s\n", bufor);
    }while(bufor[0]!='R');
    bufor[0] = 'p';
    hErr(send(sock, bufor, 1, 0)-1, "SEND");
    do {
        hErr(recv(sock, bufor, sizeof(bufor), 0)-1, "RECV");
        printf("%s\n", bufor);
    }while(bufor[0]!='P');
    bufor[0] = 't';
    hErr(send(sock, bufor, 1, 0)-1, "SEND");
    do {
        hErr(recv(sock, bufor, sizeof(bufor), 0)-1, "RECV");
        printf("%s\n", bufor);
    }while(bufor[0]!='T');
    bufor[0] = 'c';
    hErr(send(sock, bufor, 1, 0)-1, "SEND");
    do {
        hErr(recv(sock, bufor, sizeof(bufor), 0)-1, "RECV");
        printf("%s\n", bufor);
    }while(bufor[0]!='C');
    hErr(shutdown(sock, SHUT_RDWR), "SHUTDOWN");
}

void server() {
    const short port = PORT;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htons( INADDR_ANY );
    const int sock = hErr(socket(AF_INET, SOCK_STREAM, 0), "SOCKET");
    int true = 1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int));
    hErr(bind(sock, (struct sockaddr*) &addr, sizeof(addr)), "BIND");
    hErr(listen(sock, MAX_CONNECTION), "LISTEN");
    socklen_t len;
    struct sockaddr_in client = {};
    int csock = hErr(accept(sock, (struct sockaddr*) &client, &len), "ACCEPT");
    char bufor[16];
    do {
        hErr(recv(csock, bufor, sizeof( bufor ), 0)-1, "RECV");
        printf("%s\n", bufor);
        bufor[0] += 'A' - 'a';
        hErr(send(csock, bufor, 1, 0)-1, "SEND");
    }while(bufor[0]!='C');
    hErr(shutdown(sock, SHUT_RDWR), "SHUTDOWN");
}

int main(int argc, char* argv[]) {
    printf("Type 'c' to run client or 's' to run server: ");
    char c = getchar();
    wait:
    if(c=='c') client();
    else if(c=='s')server();
    else goto wait;
    return 0;
}
