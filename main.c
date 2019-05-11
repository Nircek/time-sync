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
    if(code==-1) {
        fprintf(stderr, "%s\n", msg);
        exit(1);
    }
    return code;
}

void client() {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    const char *host = "localhost";
    unsigned short port = PORT;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host, &addr.sin_addr);
    const int sock = hErr(socket(AF_INET, SOCK_STREAM, 0), "SOCKET");
    hErr(connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)), "CONNECT");
    char bufor[256] = "...";
    printf("%li ", send(sock, bufor, 3, 0));
    printf("%li ", recv(sock, bufor, sizeof(bufor), 0));
    printf("%s\n", bufor);
    hErr(shutdown(sock, SHUT_RDWR), "SHUTDOWN");
}

void server() {
    const short port = PORT;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htons( INADDR_ANY );
    const int sock = hErr(socket(AF_INET, SOCK_STREAM, 0), "SOCKET");
    hErr(bind(sock, (struct sockaddr*) &addr, sizeof(addr)), "BIND");
    listen(sock, MAX_CONNECTION);
    socklen_t len;
    struct sockaddr_in client = {};
    int csock = hErr(accept(sock, (struct sockaddr*) &client, &len), "ACCEPT");
    char bufor[256];
    printf("%li ", recv( csock, bufor, sizeof( bufor ), 0 ));
    printf("%s\n", bufor);
    printf("%li ", send( csock, bufor, 3, 0 ));
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
