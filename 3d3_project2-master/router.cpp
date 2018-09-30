#include <string.h>
#include <stdio.h>
#include <iostream>
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
     result_table->add(i,node,10000,'_');
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
   //third step
   for (int e=0;e<edges->getSize();e++) {
     edge_u = edges->getEdgeU(e);
     edge_v = edges->getEdgeV(e);
     weight = edges->getEdgeWeight(e);
     edge_u_distance = result_table->getDistance(edge_u);
     edge_v_distance = result_table->getDistance(edge_v);

     if((edge_u_distance + weight) < edge_v_distance) {
       std::cout << "Error: negative-weight cycle." << std::endl;
       exit(1);
     }
   }

   return result_table;
}

int main(int argc, const char* argv[]){
  //int src_index = atoi(argv[1]); //index of self

  VertexList *vertices = new VertexList;
  vertices->add('a', "127.0.0.1", 4000);
  vertices->add('b', "127.0.0.1", 4001);
  vertices->add('c', "127.0.0.1", 4002);
  vertices->add('d', "127.0.0.1", 4003);

  EdgeList *new_list = new EdgeList;

  new_list->add(0, 1, 3);
  new_list->add(0, 2, 17);
  new_list->add(0, 3, 2);
  new_list->add(1, 2, 5);
  new_list->add(1, 3, 4);
  new_list->add(2, 3, 3);


  RoutingTable *routing_table = compute_table(new_list, vertices, 0);
  vertices->displayVertices();
  std::cout << "" << std::endl;
  routing_table->displayTable("Converged table:");
}
