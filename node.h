#ifndef NODE_H
#define NODE_H

#include <QSet>
#include <QHash>

class Node
{
public:
    Node();
    void debugPrint(Node& node);
    void addRelation(Node& destination, QString value);
    bool operator==(const Node &other) const;

private:
    QHash<Node, QSet<QString>*>* connections;
    QHash<Node, QSet<QString>*>* reverseConnections;
};

#endif // NODE_H
