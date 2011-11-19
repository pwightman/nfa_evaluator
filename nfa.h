#ifndef NFA_H
#define NFA_H

#include <QHash>
#include <QSet>

class Nfa
{
public:
    static Nfa* createNfa(QSet<QString*>* Q, QString* q0, QSet<QString*>* sigma, QHash<QPair<QString*, QString*>*, QSet<QString*>*>* delta, QSet<QString*>* f);

    // TODO: Figure out how to make private.
    Nfa(QSet<QString*>* Q, QString* q0, QSet<QString*>* sigma, QHash<QPair<QString*, QString*>*, QSet<QString*>*>* delta, QSet<QString*>* f);

private:
    // Private member variables.
    QSet<QString*>* Q;
    QString* q0;
    QSet<QString*>* sigma;
    QHash<QPair<QString*, QString*>*, QSet<QString*>*>* delta;
    QSet<QString*>* f;
    Nfa();

    // Private methods.
    QSet<QString*>* runNfa();
};

#endif // NFA_H
