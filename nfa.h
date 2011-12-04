#ifndef NFA_H
#define NFA_H

#include <QHash>
#include <QSet>
#include <QString>
#include <QPair>
#include "traversal.h"
#include "node.h"

class Nfa
{
public:
    // Nfa structure.
    Nfa();
    void addTransition(Node& source, Node& destination, QString value);
    void makeInitial(Node& node);
    void makeFinal(Node& node);
    void unite(Nfa& nfa);
    void concatenate(Nfa& nfa);
    void star();
    void* finals();
    void* initial();

    QSet<Node*>* runNfa(QString string);

    // Nfa methods.
    bool isValidString(QString string, bool isParallel);

private:
    // Private member variables.
    Node* q0;
    QSet<Node*>* f;
    QSet<Node*>* traverse(Node* node, QString* str, int direction);
    // NOTE: Delta was removed because it's no longer needed.


    /*QSet<QString>* traverse(QSet<Traversal*>* traversal, QString* str);
    QSet<Traversal*>* setupInitials(Traversal* trav);*/

    // Private methods.
    // void debugPrintDelta();
    // void debugPrintSet(QSet<QString>* set);
    // QSet<QString>* runNfa(QString string);
    // QSet<QString>* runNfaP(QString string);
};

#endif // NFA_H
