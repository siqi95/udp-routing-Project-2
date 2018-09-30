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
     result_table->add(i,node,1000000,'_');
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

/*
std::string array_int_as_strings(int array[], int size){
    std::string array_string = "";
    for(int i=0; i<size-1; i++){
        array_string += (to_string(array[i]) + ',');
    }    
    array_string += to_string(array[size-1]);
    return array_string;
}*/

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
  
  char* locationArray = routing_table->makeInitialArray(); //puts all location vertices into an array


  int distanceArray[vertices->getSize()];
  routing_table->makeDistanceArray(distanceArray, vertices->getSize());  // puts distances to each location into an array 


  sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(8000);
  inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); //127.0.0.1 because only running on local host 
  int socket_out = socket(AF_INET, SOCK_DGRAM, 0);

  /*
  Here, we create a message buffer in format:
  source_vertex, location[i] for source vertex, distance to location[i] from source vertex, location[i+1], distance[i+1], etc
  */
  std::string buffer = messageBuffer(argv[2], distanceArray, locationArray, vertices->getSize());
  std::cout<< buffer << std::endl;
  


  int sendOK = sendto(socket_out, buffer.c_str(), buffer.size()+1, 0, (sockaddr*)&server, sizeof(server)); 
  if (sendOK == -1){
    std::cout << "error send" << std::endl;
  }

  close(socket_out);
  return 0;
}
