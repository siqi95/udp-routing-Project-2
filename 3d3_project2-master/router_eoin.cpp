#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
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

  for(int j=0; j<6; j++){
  std::cout << test[j] << std::endl;
  }

  std::cout << std::endl;

  int test2[6];
  routing_table->makeDistanceArray(test2, 6); 
  for(int l=0; l<6; l++){
    std::cout << test2[l] << std::endl;
  }
}
