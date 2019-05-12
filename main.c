#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/timeb.h>
#include <time.h>

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

typedef int nsock;
struct ntime_t {
    uint64_t time;
    int32_t ntime;
};

void ninit() {}
void ncleanup() {}
nsock nsocket() {
    return hErr(socket(AF_INET, SOCK_STREAM, 0), "nSOCKET");
}
void nconnect(nsock sock, const char *name, unsigned short port) {
    struct hostent *he = gethostbyname(name);
    hErr(he==NULL?-1:0, "nGETHOSTBYNAME");
    const char *host = inet_ntoa(**(struct in_addr**)he->h_addr_list);
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host, &addr.sin_addr);
    hErr(connect(sock, (struct sockaddr*)&addr, sizeof(addr)), "nCONNECT");
    
}
void nsendm(nsock sock, void* msg, size_t how_many) {
    hErr(send(sock, msg, how_many, 0)-how_many, "nSEND");
}
void nsend(nsock sock, void* msg) {
    nsendm(sock, msg, sizeof(msg));
}
void nsendll(nsock sock, uint64_t msg) {
    nsend(sock, &msg);
}
void nsendl(nsock sock, uint32_t msg) {
    nsend(sock, &msg);
}
void nsends(nsock sock, uint16_t msg) {
    nsend(sock, &msg);
}
void nsendc(nsock sock, uint8_t msg) {
    nsend(sock, &msg);
}
void nsendnt(nsock sock, struct ntime_t msg) {
    nsend(sock, &msg.time);
    nsend(sock, &msg.ntime);
}
void nrecvm(nsock sock, void* msg, size_t how_many) {
    hErr(recv(sock, msg, how_many, 0)-how_many, "cRECV");
}
void nrecv(nsock sock, void* msg) {
    nrecvm(sock, msg, sizeof(msg));
}
uint64_t nrecvll(nsock sock) {
    uint64_t r;
    nrecv(sock, &r);
    return r;
}
uint32_t nrecvl(nsock sock) {
    uint32_t r;
    nrecv(sock, &r);
    return r;
}
uint16_t nrecvs(nsock sock) {
    uint16_t r;
    nrecv(sock, &r);
    return r;
}
uint8_t nrecvc(nsock sock) {
    uint8_t r;
    nrecv(sock, &r);
    return r;
}
struct ntime_t nrecvnt(nsock sock) {
    struct ntime_t r;
    nrecv(sock, &r.time);
    nrecv(sock, &r.ntime);
    return r;
}
struct ntime_t ntime() {
    struct timespec ts={0,0};
    clock_gettime(CLOCK_REALTIME, &ts);
    struct ntime_t r;
    r.time = ts.tv_sec;
    r.ntime = ts.tv_nsec;
    return r;
}
double doublent(struct ntime_t nt) {
    return nt.time+1e-9*nt.ntime;
}
double diffdoublent(struct ntime_t nt1, struct ntime_t nt2) {
    return (nt1.time-nt2.time) + 1e-9*(nt1.ntime-nt2.ntime);
}
void nbind(nsock sock, short port) {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htons( INADDR_ANY );
    int true = 1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int));
    hErr(bind(sock, (struct sockaddr*) &addr, sizeof(addr)), "nBIND");
}
void nlisten(nsock sock, int backlog) {
    hErr(listen(sock, backlog), "nLISTEN");
}
nsock naccept(nsock sock) {
        socklen_t len;
        struct sockaddr_in client = {};
        return hErr(accept(sock, (struct sockaddr*) &client, &len), "nACCEPT");
}
void nclose(nsock sock) {
    hErr(shutdown(sock, SHUT_RDWR), "nSHUTDOWN");
}

void client() {
    nsock sock = nsocket();
    nconnect(sock, "localhost", PORT);
    char bufor[16] = "r";
    nsendc(sock, 'r');
    do {
        hErr(recv(sock, bufor, 1, 0)-1, "cRECV");
        printf("%s\n", bufor);
    }while(bufor[0]!='R');
    struct timespec tx={0,0}, tz={0,0};
    struct timeb txs, ty;
    bufor[0] = 'p';
    clock_gettime(CLOCK_MONOTONIC, &tx);
    hErr(ftime(&txs), "cFTIME");
    hErr(send(sock, bufor, 1, 0)-1, "cSEND");
    do {
        hErr(recv(sock, bufor, 1, 0)-1, "cRECV");
        printf("%s\n", bufor);
    }while(bufor[0]!='P');
    clock_gettime(CLOCK_MONOTONIC, &tz);
    bufor[0] = 't';
    hErr(send(sock, bufor, 1, 0)-1, "cSEND");
    do {
        hErr(recv(sock, bufor, 1, 0)-1, "cRECV");
        printf("%s\n", bufor);
    }while(bufor[0]!='T');
    hErr(recv(sock, &ty.time, sizeof(time_t), 0)-sizeof(time_t), "time_t cRECV");
    hErr(recv(sock, &ty.millitm, sizeof(unsigned short), 0)-sizeof(unsigned short), "ntime_t cRECV");
    bufor[0] = 'c';
    hErr(send(sock, bufor, 1, 0)-1, "cSEND");
    do {
        hErr(recv(sock, bufor, 1, 0)-1, "cRECV");
        printf("%s\n", bufor);
    }while(bufor[0]!='C');
    hErr(shutdown(sock, SHUT_RDWR), "cSHUTDOWN");
    double d = ((tz.tv_sec-tx.tv_sec)+1e-9*(tz.tv_nsec-tx.tv_nsec))/2;
    double lty = (txs.time+1e-9*txs.millitm)+d;
    double rty = (ty.time+1e-9*ty.millitm);
    double delta = rty - lty;
    printf("tx = %li.%09li\ntz = %li.%09li\ndelta = %.6f +-%.6f ms\n", tx.tv_sec, tx.tv_nsec, tz.tv_sec, tz.tv_nsec, delta*1000, d*1000);
}

void server() {
    const short port = PORT;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htons( INADDR_ANY );
    const int sock = hErr(socket(AF_INET, SOCK_STREAM, 0), "sSOCKET");
    int true = 1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int));
    hErr(bind(sock, (struct sockaddr*) &addr, sizeof(addr)), "sBIND");
    hErr(listen(sock, MAX_CONNECTION), "sLISTEN");
    while(1) {
        socklen_t len;
        struct sockaddr_in client = {};
        int csock = hErr(accept(sock, (struct sockaddr*) &client, &len), "sACCEPT");
        char bufor[1];
        struct timeb ty;
        do {
            hErr(recv(csock, bufor, 1, 0)-1, "sRECV");
            if(bufor[0]=='p')
                ftime(&ty);
            bufor[0] += 'A' - 'a';
            hErr(send(csock, bufor, 1, 0)-1, "sSEND");
            if(bufor[0]=='T') {
                hErr(send(csock, &ty.time, sizeof(time_t), 0)-sizeof(time_t), "time_t sSEND");
                hErr(send(csock, &ty.millitm, sizeof(unsigned short), 0)-sizeof(unsigned short), "ntime_t sSEND");
            }
            printf("%s\n", bufor);
        }while(bufor[0]!='C');
        hErr(shutdown(csock, SHUT_RDWR), "sSHUTDOWN");
    }
    hErr(shutdown(sock, SHUT_RDWR), "sSHUTDOWN");
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
