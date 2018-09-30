#include "RoutingTable_eoin.h"
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string>

void RoutingTable::add(int index, char val, int distance, char pred) {
  Entry * n = new Entry;
  n->index = index;
  n->initial = val;
  n->distance = distance;
  n->predecessor = pred;
  n->next = head;
  head = n;
  size++;
}

int RoutingTable::getSize() {
  return size;
}

int RoutingTable::getDistance(int index) {
  Entry *n = head;
  int i = 0;
  while(i < index) {
      n = n->next;
      i++;
  }
  return n->distance;
}

char RoutingTable::getInitial(int index) {
  Entry *n = head;
  int i = 0;
  while(i < index) {
      n = n->next;
      i++;
  }
  return n->initial;
}

char RoutingTable::getPred(int index) {
  Entry *n = head;
  int i = 0;
  while(i < index) {
      n = n->next;
      i++;
  }
  return n->predecessor;
}

void RoutingTable::setDistance(int index, int dist) {
  Entry *n = head;
  int i = 0;
  while(i < index) {
      n = n->next;
      i++;
  }
  n->distance = dist;
}

void RoutingTable::setPred(int index, char pred) {
  Entry *n = head;
  int i = 0;
  while(i < index) {
      n = n->next;
      i++;
  }
  n->predecessor = pred;
}

void RoutingTable::displayTable(std::string label, char a) {
  Entry *n = head;
  std::cout << label << a <<"\n--------------" << std::endl;
  for(int r=0;r<size;r++) {
    std::cout << "Location: " << n->initial
              << " Predecessor: " << n->predecessor
              << " Distance: " << n->distance << std::endl;
    n = n->next;
  }
  std::cout << "\n" << std::endl;
}

char* RoutingTable::makeInitialArray(){  
  Entry *n = head;
  char *initial = (char *) malloc(sizeof(char) * 6);
  for(int r=0;r<size;r++) {
    initial[r] = n->initial; 
    n = n->next;
  }
  return initial;
}  


void RoutingTable::makeDistanceArray(int distance[], int size){
  Entry *n = head;
  for(int r=0;r<size;r++) {
    distance[r] = n->distance; 
    n = n->next;
  } 
}
