#ifndef NFA_H
#define NFA_H

#define VERBOSE 1

#include <QHash>
#include <QSet>
#include <QString>
#include <QPair>
#include "traversal.h"
#include "node.h"

class Nfa
{
public:
    Nfa();
    void addTransition(Node& source, Node& destination, QString path);
    void makeInitial(Node& node);
    void makeFinal(Node& node);
    bool isValidString(QString string, bool isParallel);

    /*
     * Prints a string that python can read in as its own native data structures
     * (Used for testing)
     */
    QString toPyString();

private:
    // Private member variables.
    QSet<QString>* Q;
    QString q0;
    QSet<QString>* sigma;
    QHash<QPair<QString, QString>, QSet<QString>*>* delta;
    QHash<QPair<QString, QString>, QSet<QString>*>* reverseDelta;
    QSet<QString>* f;

    QSet<QString>* traverse(QSet<Traversal*>* traversal, QString* str);
    QSet<Traversal*>* setupInitials(Traversal* trav);

    // Private methods.
    void debugPrintDelta();
    void debugPrintSet(QSet<QString>* set);
    QSet<QString>* runNfa(QString string);
    QSet<QString>* runNfaP(QString string);
};

#endif // NFA_H
