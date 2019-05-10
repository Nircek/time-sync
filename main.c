#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>

int hErr(int code, const char* msg) { //handle error
    if(code==-1) {
        fprintf(stderr, "%s\n", msg);
        exit(1);
    }
    return code;
}

int main(int argc, char* argv[]) {
    printf("Hello world\n");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons( 2323 );
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    const int sock = hErr(socket(AF_INET, SOCK_STREAM, 0), "SOCKET");
    hErr(connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)), "CONNECT");
    char bufor[ 256 ] = "...";
    printf("%li ", send( sock, bufor, 3, 0 ));
    printf("%li ", recv( sock, bufor, sizeof( bufor ), 0 ));
    printf("%c\n", bufor[0]);
    hErr(shutdown(sock, SHUT_RDWR), "SHUTDOWN(-1)");
}
