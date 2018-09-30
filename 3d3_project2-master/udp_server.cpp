#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
 
using namespace std;

int main(){
int socket_in = socket(AF_INET, SOCK_DGRAM, 0); 

struct sockaddr_in serverAddr; 
struct sockaddr_in clientAddr; 
//sockaddr &serverAddrCast = (sockaddr &) serverAddr;
//sockaddr &clientAddrCast = (sockaddr &) clientAddr;

serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(8000);
serverAddr.sin_addr.s_addr = INADDR_ANY;

bind (socket_in, (sockaddr*)&serverAddr, sizeof(serverAddr));

socklen_t clientLength = sizeof(clientAddr);

char buf[1024] = {}; // may not want to initialise it
int bufferSize = sizeof(buf);

while(1){
//cout << "listening" << endl;
int recvOK = recvfrom(socket_in, buf, 1024, 0, (sockaddr*)&clientAddr, &clientLength);
if(recvOK == -1){
    cout <<  "error reveive" << endl;
}
//sendto(socket_s, buf, 1024, 0, (sockaddr*)&clientAddr, clientLength);

char clientIP[256] = {}; // may not want to initialise it 

inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, 256);

cout << "message received from " << clientIP << " : " << buf << endl;

}

close(socket_in);


return 0;
}
