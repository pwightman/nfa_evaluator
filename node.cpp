#include "node.h"
#include "stdio.h"
#include <QSetIterator>

Node::Node()
{
    connections = new QHash<Node, QSet<QString>*>();
    reverseConnections = new QHash<Node, QSet<QString>*>();
}

void Node::addRelation(Node& destination, QString value)
{
    // Add forward traversal relation.
    if (connections->contains(destination))
    {
        QSet<QString>* set = connections->value(destination);
        set->insert(value);
    }
    else
    {
        QSet<QString>* set = new QSet<QString>();
        set->insert(value);
        connections->insert(destination, set);
    }

    // Add backward traversal relation.
    if (destination.reverseConnections->contains(*this))
    {
        QSet<QString>* set = destination.reverseConnections->value(*this);
        set->insert(value);
    }
    else
    {
        QSet<QString>* set = new QSet<QString>();
        set->insert(value);
        destination.reverseConnections->insert(*this, set);
    }
}

void Node::debugPrint(Node& node)
{
    if (connections->contains(node))
    {
        QSetIterator<QString> i(*connections->value(node));

        printf("[ ");
        while (i.hasNext())
        {
            printf("%s ", i.next().toStdString().c_str());
        }
        printf("]\n");
    }
    else
    {
        printf("No forward connections.\n");
    }


    if (reverseConnections->contains(node))
    {
        QSetIterator<QString> i(*reverseConnections->value(node));

        printf("[ ");
        while (i.hasNext())
        {
            printf("%s ", i.next().toStdString().c_str());
        }
        printf("]\n");
    }
    else
    {
        printf("No reverse connections.\n");
    }
    printf("\n");
}

int qHash(const Node& node)
{
    return (int)(long)&node;
}

bool Node::operator==(const Node &other) const
{
    return connections == other.connections && reverseConnections == other.reverseConnections;
}
