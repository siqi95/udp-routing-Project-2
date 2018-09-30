#include <stddef.h>
#include <string>

class RoutingTable {
  private:
    struct Entry {
      int index;
      char initial;
      int distance;
      char predecessor;
      Entry * next;
    };
    int size;
    Entry * head;
  public:
    RoutingTable() {
          head = NULL;
          size = 0;
    }
    ~RoutingTable() {
          while(head != NULL) {
              Entry * n = head->next;
              delete head;
              head = n;
          }
    }
    int getSize();
    void add(int index, char val, int distance, char pred);
    void updateEntry(int distance, char pred);
    int getDistance(int index);
    char getInitial(int index);
    char getPred(int index);
    void setDistance(int index, int dist);
    void setPred(int index, char pred);
    void displayTable(std::string label);
};
