#include "node.h"
#include "stdio.h"
#include <QSetIterator>
#include <QHashIterator>

Node::Node()
{
    connections = new QHash<QString, QSet<Node>*>();
    reverseConnections = new QHash<QString, QSet<Node>*>();
}

Node::Node(QString name)
{
    connections = new QHash<QString, QSet<Node>*>();
    reverseConnections = new QHash<QString, QSet<Node>*>();
    this->name = name;
}

QString Node::getName()
{
    return name;
}

void Node::addRelation(Node& destination, QString value)
{
    printf("Checking to see if %s is in the hash of node %s.\n", value.toStdString().c_str(), destination.getName().toStdString().c_str());
    // Add forward traversal relation.
    if (connections->contains(value))
    {
        QSet<Node>* set = connections->value(value);
        set->insert(destination);
    }
    else
    {
        printf("%s wasn't found in the hash, so creating a new entry.\n", value.toStdString().c_str());
        QSet<Node>* set = new QSet<Node>();
        set->insert(destination);
        connections->insert(value, set);
    }

    // Add backward traversal relation.
    if (destination.reverseConnections->contains(value))
    {
        QSet<Node>* set = destination.reverseConnections->value(value);
        set->insert(*this);
    }
    else
    {
        QSet<Node>* set = new QSet<Node>();
        set->insert(*this);
        destination.reverseConnections->insert(value, set);
    }
    printf("---\n");
}

void Node::debugPrint()
{
    if (name == "")
    {
        printf("Debug print won't work because name hasn't been set.\n");
        return;
    }

    QHashIterator<QString, QSet<Node>*> i(*connections);
    while(i.hasNext())
    {
        i.next();
        QString string = i.key();
        QSet<Node>* set = i.value();
        printf("%s -> [ ", string.toStdString().c_str());
        QSetIterator<Node> j(*set);
        while (j.hasNext())
        {
            printf("%s ", j.next().name.toStdString().c_str());
        }
        printf("]\n");
    }

    /*if (connections->contains(node))
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
    printf("\n");*/
}

int qHash(const Node& node)
{
    return (int)(long)&node;
}

bool Node::operator==(const Node &other) const
{
    return connections == other.connections && reverseConnections == other.reverseConnections;
}
