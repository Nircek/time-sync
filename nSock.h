#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/timeb.h>
#include <time.h>

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

int hErr(int code, const char* msg); //handle error

typedef int nsock;
struct ntime_t {
    uint64_t time;
    int32_t ntime;
};

void ninit();
void ncleanup();
nsock nsocket();
void nconnect(nsock, const char*, unsigned short);
void nsend(nsock, void*, size_t);
void nsendll(nsock, uint64_t);
void nsendl(nsock, uint32_t);
void nsends(nsock, uint16_t);
void nsendc(nsock, uint8_t);
void nsendnt(nsock, struct ntime_t);
void nrecv(nsock, void*, size_t);
uint64_t nrecvll(nsock);
uint32_t nrecvl(nsock);
uint16_t nrecvs(nsock);
uint8_t nrecvc(nsock);
struct ntime_t nrecvnt(nsock);
struct ntime_t ntime();
double doublent(struct ntime_t);
double diffdoublent(struct ntime_t, struct ntime_t);
void nbind(nsock, short);
void nlisten(nsock, int);
nsock naccept(nsock sock);
void nclose(nsock sock);