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
#include "EdgeList_eoin.h"
#include "RoutingTable_eoin.h"
#include "VertexList_eoin.h" 


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
         //result_table->displayTable("Routing Table:", src_index);
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
        x++;
      }
      array_b[i] = atoi(t_string.c_str());
      t_string = "";
      x++;
    }
}

bool realUpdate(EdgeList *edges, char source, char array_a[], int array_b[], int size){
  bool checker;
  bool checker2; // directional
  bool updated = false;
  for(int i=0; i<size; i++){
    checker = edges->checkEdge(indexOf(source), indexOf(array_a[i]), array_b[i]);
    checker2 = edges->checkEdge(indexOf(array_a[i]), indexOf(source), array_b[i]);
    if(!checker){
      edges->add(indexOf(source), indexOf(array_a[i]), array_b[i]);
      std::cout << "edge added: " << indexOf(source) << ", " << indexOf(array_a[i]) << std::endl;
      bool updated = true;
    }
    if(!checker2){
      edges->add(indexOf(array_a[i]), indexOf(source), array_b[i]);
      std::cout << "edge added: " << indexOf(array_a[i]) << ", "  << indexOf(source) << std::endl;
      bool updated = true;
    }
  }
  return updated;
}


std::string convertInt(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

std::string messageBuffer(char* source, int array_a[], char array_b[], int size){
    std::string val;
    val = source[0];
    for(int i=0; i<size-1; i++){
        val = val + ',' + array_b[i] + ',' + convertInt(array_a[i]);
    }
    val = val + ',' + array_b[size-1] + ',' + convertInt(array_a[size-1]) + "            ";
    return val;
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
  VertexList *neighbours = new VertexList;

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
      neighbours->add(name_v, "127.0.0.1", port_v);
    }

  }
  for(int y = 0; y < array_size; y++){
      vertices->add(name_array[y], "127.0.0.1", port_array[y]);
  }

  vertices->displayVertices();
  RoutingTable *routing_table = compute_table(new_list, vertices, tableNameIndex);

  std::cout << "" << std::endl;
  routing_table->displayTable("Converged table:", tableName);
  
  //ARRAY OF LOCATIONS FROM ROUTING TABLE
  char* test = routing_table->makeInitialArray(); 


  std::cout << std::endl;
  int test3[vertices->getSize()];
  routing_table->makeDistanceArray(test3, vertices->getSize()); 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int socket_in = socket(AF_INET, SOCK_DGRAM, 0); 

struct sockaddr_in serverAddr; 
struct sockaddr_in clientAddr; 
//sockaddr &serverAddrCast = (sockaddr &) serverAddr;
//sockaddr &clientAddrCast = (sockaddr &) clientAddr;
int myPort = port_array[indexOf(tableName)];
serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(myPort);
serverAddr.sin_addr.s_addr = INADDR_ANY;

bind (socket_in, (sockaddr*)&serverAddr, sizeof(serverAddr));

socklen_t clientLength = sizeof(clientAddr);


int pid = fork();

if(pid == -1){
    perror("fork failed\n");
}

else if(pid > 0){

for(;;){
    //cout << "listening" << endl;
    char buf[2048] = {}; // may not want to initialise it
    int bufferSize = sizeof(buf);
    int recvOK = recvfrom(socket_in, buf, 2048, 0, (sockaddr*)&clientAddr, &clientLength);
    if(recvOK == -1){
        std::cout <<  "error reveive" << std::endl;
    }
    //sendto(socket_s, buf, 1024, 0, (sockaddr*)&clientAddr, clientLength);

    char clientIP[256] = {}; // may not want to initialise it 

    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, 256);



    //std::cout << "message received from " << clientIP << " : " << buf << std::endl;

    messageDecoder(buf, buf[0], test,  test3, vertices->getSize());
    //std::cout << "MESSAGE: " << buf[0] << std::endl;
    bool update = false;
    update = realUpdate(new_list, buf[0], test, test3, vertices->getSize());


    //if(update){
    routing_table = compute_table(new_list, vertices, tableNameIndex);
    //std::cout << "gfuf" << std::endl;
    routing_table->displayTable("New Table by:", buf[0]);
    //}

    sleep(1);
}

}

else{

    //int socket_out = socket(AF_INET, SOCK_DGRAM, 0);
  for(;;){
    
    char* locationArray = routing_table->makeInitialArray(); //puts all location vertices into an array
    int distanceArray[vertices->getSize()];
    routing_table->makeDistanceArray(distanceArray, vertices->getSize());  // puts distances to each location into an array 
    std::string buffer = "";
    buffer = messageBuffer(argv[2], distanceArray, locationArray, vertices->getSize());

    routing_table = compute_table(new_list, vertices, tableNameIndex);
    for(int h=0; h < neighbours->getSize(); h++){
        
        sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons(neighbours->getNeighbourPort(h));
        inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); //127.0.0.1 because only running on local host 

        int sendOK = sendto(socket_in, buffer.c_str(), buffer.size()+1, 0, (sockaddr*)&server, sizeof(server)); 
       if (sendOK == -1){
            //std::cout << neighbours->getNeighbourPort(h) << std::endl;
        std::cout << "error send" << std::endl;
        }
    }
  }
}
//close(socket_in);


return 0;
}
