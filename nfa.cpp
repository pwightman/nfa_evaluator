#include "nfa.h"
#include "stdio.h"

Nfa::Nfa(QSet<QString*>* Q, QString* q0, QSet<QString*>* sigma, QHash<QPair<QString*, QString*>*, QSet<QString*>*>* delta, QSet<QString*>* f)
{
    this->Q = Q;
    this->q0 = q0;
    this->sigma = sigma;
    this->delta = delta;
    this->f = f;
}

Nfa* Nfa::createNfa(QSet<QString*>* Q, QString* q0, QSet<QString*>* sigma, QHash<QPair<QString*, QString*>*, QSet<QString*>*>* delta, QSet<QString*>* f)
{
    return new Nfa(Q, q0, sigma, delta, f);
}

QSet<QString*>* Nfa::runNfa()
{
    Set<QString*>* qSet = new QSet<QString*>();
    qSet->insert(q0);

    QPair<QString*, QString*>* pair = new QPair<QString*, QString*>();
    pair->first = q0;
    pair->second = new QString("@");
    if (delta->contains(pair))
    {
        printf("Got here");
    }
}
