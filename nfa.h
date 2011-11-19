#ifndef NFA_H
#define NFA_H

#include <QHash>
#include <QSet>
#include <QString>
#include <QPair>

class Nfa
{
public:
    static Nfa* createNfa(QSet<QString>* Q, QString q0, QSet<QString>* sigma, QHash<QPair<QString, QString>*, QSet<QString>*>* delta, QSet<QString>* f);

    // TODO: Figure out how to make private.
    Nfa(QSet<QString>* Q, QString q0, QSet<QString>* sigma, QHash<QPair<QString, QString>*, QSet<QString>*>* delta, QSet<QString>* f);

    // TODO: Make private again.
    QSet<QString>* runNfa();

private:
    // Private member variables.
    QSet<QString>* Q;
    QString q0;
    QSet<QString>* sigma;
    QHash<QPair<QString, QString>*, QSet<QString>*>* delta;
    QSet<QString>* f;
    Nfa();
};

#endif // NFA_H
