#include "traversal.h"

Traversal::Traversal(int stringPos, Node& state)
{
  _stringPos = stringPos;
  _state = &state;
}

/* Getter/setter stringLeft */
int Traversal::stringPos()
{
  return _stringPos;
}

void Traversal::setStringPos(int newStringPos)
{
  _stringPos = newStringPos;
}

/* Getter/setter state */
Node* Traversal::state()
{
  return _state;
}

void Traversal::setState(Node& newState)
{
  _state = &newState; 
}
