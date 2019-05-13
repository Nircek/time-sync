#include <stdlib.h>
#include <stdio.h>
#include "nSock.h"

#define PORT 10001
#define MAX_CONNECTION 1

void client() {
    nsock sock = nsocket();
    nconnect(sock, "localhost", PORT);
    nsendc(sock, 'r');
    while(nrecvc(sock) != 'R');
    struct ntime_t tx, ty, tz;
    tx = ntime();
    nsendc(sock, 'p');
    while(nrecvc(sock) != 'P');
    tz = ntime();
    nsendc(sock, 't');
    while(nrecvc(sock) != 'T');
    ty = nrecvnt(sock);
    nsendc(sock, 'c');
    while(nrecvc(sock) != 'C');
    nclose(sock);
    ncleanup();
    double d = diffdoublent(tx, tz)/2;
    double lty = doublent(tx)+d;
    double rty = doublent(ty);
    double delta = rty - lty;
    printf("tx = %li.%09li\nty = %li.%09li\ntz = %li.%09li\ndelta = %.6f +-%.6f ms\n", tx.time, tx.ntime, ty.time, ty.ntime, tz.time, tz.ntime, delta*1000, d*1000);
}

void server() {
    nsock sock = nsocket();
    nbind(sock, PORT);
    nlisten(sock, MAX_CONNECTION);
    while(1) {
        nsock csock = naccept(sock);
        struct ntime_t last = ntime();
        char b;
        do {
            b = nrecvc(csock);
            if(b == 'p')
                last = ntime();
            b += 'A' - 'a';
            nsendc(csock, b);
            if(b == 'T')
                nsendnt(csock, last);
            printf("%c\n", b);
        }while(b!='C');
        nclose(csock);
    }
    nclose(sock);
}

int main(int argc, char* argv[]) {
    ninit();
    printf("Type 'c' to run client or 's' to run server: ");
    char c = getchar();
    wait:
    if(c=='c') client();
    else if(c=='s')server();
    else goto wait;
    ncleanup();
    return 0;
}
