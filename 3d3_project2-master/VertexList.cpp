#include "VertexList_eoin.h"
#include <string>
#include <stdio.h>
#include <iostream>

int VertexList::getSize() {
  return size;
}

void VertexList::add(char node, std::string host, int port) {
  Vertex * n = new Vertex;
  n->node = node;
  n->host = host;
  n->port = port;
  n->next = head;
  head = n;
  size++;
}

std::string VertexList::getHost(char entry) {
  Vertex *n = head;
  int i = 0;
  while(i < size) {
      if (n->node == entry) {
        return n->host;
      } else {
        n = n->next;
        i++;
      }
  }
  return "Error: out of bounds.";
}

int VertexList::getPort(char entry) {
  Vertex *n = head;
  int i = 0;
  while(i < size) {
      if (n->node == entry) {
        return n->port;
      } else {
        n = n->next;
        i++;
      }
  }
  return -1;
}

void VertexList::displayVertices() {
  Vertex *n = head;
  std::cout << "Vertices: \n--------------" << std::endl;
  for(int r=0;r<size;r++) {
    std::cout << "Node: " << n->node
              << " Host: " << n->host
              << " Port: " << n->port << std::endl;
    n = n->next;
  }
}

int VertexList::getNeighbourPort(int index){
  Vertex *n = head;
  int i = 0;
  while(i < index) {
      n = n->next;
      i++;
  }
  return n->port;
}

char VertexList::getNode(int index) {
  Vertex *n = head;
  int i = 0;
  while(i < index) {
      n = n->next;
      i++;
  }
  return n->node;
}
