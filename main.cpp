#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

int hErr(int code, string msg) { //handle error
    if(code==-1) {
        cerr<<msg<<'\n';
        exit(1);
    }
    return code;
}

int main(int argc, char* argv[]) {
    cout << "Hello world!" << endl;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons( 2323 );
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    const int sock = hErr(socket(AF_INET, SOCK_STREAM, 0), "SOCKET");
    hErr(connect(sock, (struct sockaddr*)&addr, sizeof(sockaddr_in)), "CONNECT");
    char bufor[ 256 ] = "...";
    cout<<send( sock, bufor, 3, 0 );
    cout<<recv( sock, bufor, sizeof( bufor ), 0 );
    cout << bufor[0] << endl;
    hErr(shutdown(sock, SHUT_RDWR), "SHUTDOWN(-1)");
}
