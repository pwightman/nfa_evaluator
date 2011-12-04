#ifndef TRAVERSAL_H
#define TRAVERSAL_H

#include <QString>

class Node;

class Traversal
{
  public:
    Traversal(int stringPos, Node& state);

    /* Getter/setter stringLeft */
    int stringPos();
    void setStringPos(int newStringPos);

    /* Getter/setter state */
    Node* state();
    void setState(Node& newState);

  private:
    /* Instance variables */
    Node* _state;
    int _stringPos;

    Traversal();
};

#endif // TRAVERSAL_H
