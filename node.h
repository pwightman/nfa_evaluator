#ifndef NODE_H
#define NODE_H

#include <QSet>
#include <QHash>
#include <QString>

class Node
{
public:
    Node();
    Node(QString name);
    void debugPrint();
    QString getName();
    void addRelation(Node& destination, QString value);
    bool operator==(const Node &other) const;

private:
    QHash<QString, QSet<Node>*>* connections;
    QHash<QString, QSet<Node>*>* reverseConnections;
    QString name; // Optional parameter in constructor.
};

#endif // NODE_H
