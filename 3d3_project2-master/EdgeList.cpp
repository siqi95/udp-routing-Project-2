#include "EdgeList.h"
#include <stdio.h>
#include <iostream>
#include <string>

void EdgeList::add(int u, int v, int weight) { 
    Edge * n = new Edge;
    n->u = u;
    n->v = v;
    n->weight = weight;
    n->next = head;
    head = n;
    size++;
}


int EdgeList::getSize() {
  return size;
}

int EdgeList::getEdgeWeight(int index) {
  Edge *n = head;
  int i = 0;
  while(i < index) {
      n = n->next;
      i++;
  }
  return n->weight;
}

int EdgeList::getEdgeU(int index) {
  Edge *n = head;
  int i = 0;
  while(i < index) {
      n = n->next;
      i++;
  }
  return n->u;
}

int EdgeList::getEdgeV(int index) {
  Edge *n = head;
  int i = 0;
  while(i < index) {
      n = n->next;
      i++;
  }
  return n->v;
}

bool EdgeList::checkEdge(int u, int v){
  Edge *n = head;
  std::cout << "EOINS TEST a" << std::endl; 
  bool check = false;
  while(n!=NULL){
    if(n->u == u && n->v == v){
      return true;
    }
    else{
      n = n->next;
    }
  }
  return check;
}
