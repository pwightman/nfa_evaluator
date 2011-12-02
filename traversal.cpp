#include "traversal.h"

Traversal::Traversal(int stringPos, QString* state)
{
  _stringPos = stringPos;
  _state = state;
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
QString* Traversal::state()
{
  return _state;
}
void Traversal::setState(QString* newState)
{
  _state = newState; }
