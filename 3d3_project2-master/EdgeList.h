#include <stddef.h>

class EdgeList {
  private:
    struct Edge {
      int u;
      int v;
      int weight;

      Edge * next;
    };
    int size;
    Edge * head;
  public:
    EdgeList() {
          head = NULL;
          size = 0;
    }
    ~EdgeList() {
          while(head != NULL) {
              Edge * n = head->next;
              delete head;
              head = n;
          }
      }
      int getSize();
      int getEdgeWeight(int index);
      int getEdgeU(int index);
      int getEdgeV(int index);
      void add(int u, int v, int weight);
      bool checkEdge(int u, int v, int cost); 
};
