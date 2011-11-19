#include <QtCore/QCoreApplication>
#include <QHash>
#include <QSet>

#include "nfa.h"

int main()
{
    // QCoreApplication a(argc, argv);
    // return a.exec();
    QSet<QString*>* Q = new QSet<QString*>();
    QString* q0 = new QString("");
    QSet<QString*>* sigma = new QSet<QString*>();
    QHash<QPair<QString*, QString*>*, QSet<QString*>*>* delta = new QHash<QPair<QString*, QString*>*, QSet<QString*>*>();
    QSet<QString*>* f = new QSet<QString*>();


    Nfa* nfa = Nfa::createNfa(Q, q0, sigma, delta, f);
}
