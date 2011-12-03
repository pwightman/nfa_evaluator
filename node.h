#ifndef NODE_H
#define NODE_H

// Data structures.
#include <QSet>
#include <QHash>
#include <QString>

// Traversal structures.
#include <QSetIterator>
#include <QHashIterator>

/* VERBOSE must be set to 1 for the debug statements to print out. */
#define VERBOSE 0
#define FORWARDS 0
#define BACKWARDS 1

class Node
{
public:
    /*
     * Node constructor can take in an optional parameter, name, 
     * which must be defined for any of the debug statements to print out.
     */
    Node();

    /*
     * Create a node with a name (Used for debugging, does not affect traversal at all)
     */
    Node(QString name);

    /*
     * Get the user's name, if given, return NULL otherwise
     */
    QString getName();

    /*
     * rawStates returns a set of nodes, which includes itself, and all the epsilon jumps.
     */
    QSet<Node>* rawStates(int direction); // Set of all states after e-closure.

    /*
     * traverseOn returns a set of nodes based off of the traversal of a given value.  
     * Note that rawStates slightly does this, but includes itself, where traverseOn doesn't.
     */
    QSet<Node>* traverseOn(QString value, int direction);

    /*
     * addRelation takes in a destination node, and a traversal value.  The reason I 
     * called this Relation instead of Traversal is because it adds the forward and 
     * backward relation.
     */
    void addRelation(Node& destination, QString value);

    /*
     * Two Nodes are considered equal if they have they same forward and reverse connections
     */
    bool operator==(const Node &other) const;

    // Debug methods.
    void debugPrint();


private:
    /* Paths that lead away from this node to other nodes */
    QHash<QString, QSet<Node>*>* connections;
    /* Paths that lead to this node from other nodes */
    QHash<QString, QSet<Node>*>* reverseConnections;
    QString name; // Optional parameter in constructor.

    // Helper methods.
    QHash<QString, QSet<Node>*>* getHash(int direction);
};

#endif // NODE_H
