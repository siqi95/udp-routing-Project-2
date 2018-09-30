#include <stddef.h>
#include <string>
#include <arpa/inet.h>

class VertexList {
  private:
    struct Vertex {
      char node;
      std::string host;
      int port;
      Vertex * next;
    };
    int size;
    Vertex * head;
  public:
    VertexList() {
          head = NULL;
          size = 0;
    }
    ~VertexList() {
          while(head != NULL) {
              Vertex * n = head->next;
              delete head;
              head = n;
          }
    }
    int getSize();
    void add(char node, std::string host, int port);
    char getNode(int index);
    std::string getHost(char entry);
    int getPort(char entry);
    int getNeighbourPort(int index);
    void displayVertices();
};
