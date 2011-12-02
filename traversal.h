#ifndef TRAVERSAL_H
#define TRAVERSAL_H

#include <QString>

class Traversal
{
  public:
    Traversal(int stringPos, QString* state);

    /* Getter/setter stringLeft */
    int stringPos();
    void setStringPos(int newStringPos);

    /* Getter/setter state */
    QString* state();
    void setState(QString* newState);

  private:
    /* Instance variables */
    QString* _state;
    int _stringPos;

    Traversal();
};

#endif // TRAVERSAL_H
