#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>
#include <sstream>

using namespace std; 

int main(int argc, char* argv[]){

sockaddr_in server;
server.sin_family = AF_INET;
server.sin_port = htons(8000);

inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); //127.0.0.1 because only running on local host 

int socket_out = socket(AF_INET, SOCK_DGRAM, 0);

string s(argv[1]); // first argument value 
int sendOK = sendto(socket_out, s.c_str(), s.size()+1, 0, (sockaddr*)&server, sizeof(server)); 
if (sendOK == -1){
    cout << "error send" << endl;
}

close(socket_out);
return 0;
}