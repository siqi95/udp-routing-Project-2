#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include "EdgeList.h"
#include "RoutingTable.h"
#include "VertexList.h" 


RoutingTable * compute_table(EdgeList *edges, VertexList *vertices, int src_index) {
   RoutingTable *result_table = new RoutingTable;

   char node;
   std::string host;
   int port;
   //first step
   for(int i = 0; i<vertices->getSize(); i++) {
     node = vertices->getNode(i);
     host = vertices->getHost(node);
     port = vertices->getPort(node);
     result_table->add(i,node,100,'_');
   }
   result_table->setDistance(src_index, 0);

   int edge_u;
   int edge_v;
   int edge_u_distance;
   int edge_v_distance;
   int weight;

   //second step

   for(int v=0;v<result_table->getSize();v++) {
     for (int e=0;e<edges->getSize();e++) {
       edge_u = edges->getEdgeU(e);
       edge_v = edges->getEdgeV(e);
       weight = edges->getEdgeWeight(e);
       edge_u_distance = result_table->getDistance(edge_u);
       edge_v_distance = result_table->getDistance(edge_v);

       if ((edge_u_distance + weight) < edge_v_distance) {
         result_table->setDistance(edge_v, edge_u_distance + weight);
         char u_initial_val = result_table->getInitial(edge_u);
         result_table->setPred(edge_v, u_initial_val);
         result_table->displayTable("Routing Table:");
       }
     }
   }
   return result_table;
}

int indexOf(char router)
{
	return router - 'A';
}

char nameOf(int index) 
{
	return (char)index + 'A';
}

void messageDecoder(char* message, char source, char array_a[], int array_b[], int size){
    source = message[0];
    std::cout << message[34] << std::endl;
    int x = 2;
    int y = 4;
    std::string t_string = ""; 
    for(int i=0; i<size; i++){
      while(message[x] != ','){
        array_a[i] = message[x];
        x++;
      }
        x++;
      while(message[x] != ','){
        t_string += message[x];
       // if(x == message.size()){
       // std::cout << "SIZE REACHED" << std::endl;
       // } 
        x++;
      }
      array_b[i] = atoi(t_string.c_str());
      t_string = "";
      std::cout << array_a[i] << std::endl;
      std::cout << array_b[i] << std::endl;
      x++;
    }
}

void realUpdate(EdgeList *edges, char source, char array_a[], int array_b[], int size){
  bool checker;
  bool checker2; // directional
  for(int i=0; i<size; i++){
    std::cout << "EOINS TEST" << std::endl;
    checker = edges->checkEdge(indexOf(source), indexOf(array_a[i]));
    checker2 = edges->checkEdge(indexOf(array_a[i]), indexOf(source));
    std::cout << "EOINS TEST 2" << std::endl;
    if(!checker){
      std::cout << "EOINS TEST 3" << std::endl;
      edges->add(indexOf(source), indexOf(array_a[i]), array_b[i]);
    }
    if(!checker2){
      std::cout << "EOINS TEST 4" << std::endl;
      edges->add(indexOf(array_a[i]), indexOf(source), array_b[i]);
    }
  }
}

void updateTable(RoutingTable *table, char source, char array_a[], int array_b[], int size){
    for(int i=0; i<size; i++){
        int distU = table->getDistance(indexOf(source));
        if(table->getDistance(i) > (distU + array_b[i]) && (distU + array_b[i]) != 0){
            table->setDistance(i, distU + array_b[i]);
            table->setPred(array_b[i], source);
        }
    }
    table->displayTable("new table");
}

int main(int argc, char** argv){
  //int src_index = atoi(argv[1]); //index of self

  if (argc < 3 || argc > 3)
	{
		perror("Please print 3 arguments. ./test <initialization file> <router name>\n");
		return 0;
	}

  //change these variables
  std::fstream topology(argv[1]);
  std::string line;
  std::string element;
  char *tableName_p = argv[2];
  char tableName = tableName_p[0];
  int tableNameIndex = indexOf(tableName);

  VertexList *vertices = new VertexList;

  EdgeList *new_list = new EdgeList;
  
  int array_size = 6;
  char name_array[array_size];
  int port_array[array_size];

  while(getline(topology, line)){
    std::stringstream linestream(line);
   
    getline(linestream, element, ',');
    char name_u = element[0];

    getline(linestream, element, ',');
    char name_v = element[0];
    name_array[indexOf(name_v)] = name_v;

    getline(linestream, element, ',');
    int port_v = atoi(element.c_str());
    port_array[indexOf(name_v)] = port_v; 


    getline(linestream, element, ',');
    int cost = atoi(element.c_str());

    int indexU = indexOf(name_u);
    int indexV = indexOf(name_v);

    if(name_u == tableName){
      new_list->add(indexU, indexV, cost);
    }

  }
  for(int y = 0; y < array_size; y++){
      vertices->add(name_array[y], "127.0.0.1", port_array[y]);
  }

  vertices->displayVertices();
  RoutingTable *routing_table = compute_table(new_list, vertices, tableNameIndex);

  std::cout << "" << std::endl;
  routing_table->displayTable("Converged table:");
  
  char* test = routing_table->makeInitialArray(); 

  for(int j=0; j<vertices->getSize(); j++){
  std::cout << test[j] << std::endl;
  }

  std::cout << std::endl;

  int test2[vertices->getSize()];
  routing_table->makeDistanceArray(test2, vertices->getSize()); 
  for(int l=0; l<vertices->getSize(); l++){
    std::cout << test2[l] << std::endl;
  }


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

char buf[2048] = {}; // may not want to initialise it
int bufferSize = sizeof(buf);

while(1){
//cout << "listening" << endl;
int recvOK = recvfrom(socket_in, buf, 2048, 0, (sockaddr*)&clientAddr, &clientLength);
if(recvOK == -1){
    std::cout <<  "error reveive" << std::endl;
}
//sendto(socket_s, buf, 1024, 0, (sockaddr*)&clientAddr, clientLength);

char clientIP[256] = {}; // may not want to initialise it 

inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, 256);



std::cout << "message received from " << clientIP << " : " << buf << std::endl;

messageDecoder(buf, buf[0], test,  test2, vertices->getSize());
std::cout << "MESSAGE: " << buf[0] << std::endl;
//updateTable(routing_table, 'E', test, test2, vertices->getSize());
realUpdate(new_list, buf[0], test, test2, vertices->getSize());
routing_table = compute_table(new_list, vertices, tableNameIndex);

std::cout << "" << std::endl;
routing_table->displayTable("New Table:");

}

close(socket_in);


return 0;
}
