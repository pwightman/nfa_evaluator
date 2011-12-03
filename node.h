#ifndef NODE_H
#define NODE_H

// Data structures.
#include <QSet>
#include <QHash>
#include <QString>

// Traversal structures.
#include <QSetIterator>
#include <QHashIterator>

#define VERBOSE 0
#define FORWARDS 0
#define BACKWARDS 1

class Node
{
public:
    Node();
    Node(QString name);
    QString getName();
    QSet<Node>* rawStates(int direction); // Set of all states after e-closure.
    QSet<Node>* traverseOn(QString value, int direction);
    void addRelation(Node& destination, QString value);
    bool operator==(const Node &other) const;

    // Debug methods.
    void debugPrint();

private:
    QHash<QString, QSet<Node>*>* connections;
    QHash<QString, QSet<Node>*>* reverseConnections;
    QString name; // Optional parameter in constructor.

    // Helper methods.
    QHash<QString, QSet<Node>*>* getHash(int direction);
};

#endif // NODE_H
