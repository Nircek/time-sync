#include "nSock.h"

/*
MIT License

Copyright (c) 2019 Nircek

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

int hErr(int code, const char* msg) { //handle error
    if(code<=-1) {
        fprintf(stderr, "[ERR] %s %i\n", msg, code);
        exit(1);
    } else {
        printf("[OK] %s\n", msg);
    }
    return code;
}

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
void nsend(nsock sock, void* msg, size_t how_many) {
    hErr(send(sock, msg, how_many, 0)-how_many, "nSEND");
}
void nsendll(nsock sock, uint64_t msg) {
    nsend(sock, &msg, 8);
}
void nsendl(nsock sock, uint32_t msg) {
    nsend(sock, &msg, 4);
}
void nsends(nsock sock, uint16_t msg) {
    nsend(sock, &msg, 2);
}
void nsendc(nsock sock, uint8_t msg) {
    nsend(sock, &msg, 1);
}
void nsendnt(nsock sock, struct ntime_t msg) {
    nsendll(sock, msg.time);
    nsendl(sock, msg.ntime);
}
void nrecv(nsock sock, void* msg, size_t how_many) {
    hErr(recv(sock, msg, how_many, 0)-how_many, "nRECV");
}
uint64_t nrecvll(nsock sock) {
    uint64_t r;
    nrecv(sock, &r, 8);
    return r;
}
uint32_t nrecvl(nsock sock) {
    uint32_t r;
    nrecv(sock, &r, 4);
    return r;
}
uint16_t nrecvs(nsock sock) {
    uint16_t r;
    nrecv(sock, &r, 2);
    return r;
}
uint8_t nrecvc(nsock sock) {
    uint8_t r;
    nrecv(sock, &r, 1);
    return r;
}
struct ntime_t nrecvnt(nsock sock) {
    struct ntime_t r;
    r.time = nrecvll(sock);
    r.ntime = nrecvl(sock);
    return r;
}
struct ntime_t ntime() {
    struct timespec ts;
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
