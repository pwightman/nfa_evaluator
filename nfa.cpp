#include "nfa.h"
#include "stdio.h"
#include <QHashIterator>


Nfa::Nfa()
{
    q0 = new Node();
    f = new QSet<Node>();
}

void Nfa::addTransition(Node& source, Node& destination, QString value)
{
    source.addRelation(destination, value);
}

void* Nfa::finals()
{
  return (void*)f;
}

void* Nfa::initial()
{
  return (void*)q0;
}

void Nfa::makeInitial(Node& node)
{
    q0 = &node;
}

void Nfa::makeFinal(Node& node)
{
    this->f->insert(node);
}

void Nfa::unite(Nfa& nfa)
{
    Node* newq0 = new Node("AG"); // AG: Auto generated.
    newq0->addRelation(*q0, "@");
    newq0->addRelation(*nfa.q0, "@");
    q0 = newq0;
}

void Nfa::concatenate(Nfa& nfa)
{
    // Make all the final states transition to nfa.q0
    QSetIterator<Node> i(*f);
    while(i.hasNext())
    {
        Node node = i.next();
        node.addRelation(*nfa.q0, "@");
    }

    // Make this node's final states the concatentated final states.
    f->clear();
    f->unite(*nfa.f); // Done this way for memory management.
    // NOTE: Initial state remains the same.
}

void Nfa::star()
{
    Node* newq0 = new Node("AG"); // AG: Auto generated.
    newq0->addRelation(*q0, "@");

    // Link all final states to q0.
    QSetIterator<Node> i(*f);
    while (i.hasNext())
    {
        Node node = i.next();
        node.addRelation(*q0, "@");
    }

    // Make newq0 the new q0
    q0 = newq0;
}

static int qHash(const Node& node)
{
    return (int)(long)&node;
}

/*void Nfa::debugPrintDelta()
{
    QHashIterator<QPair<QString, QString>, QSet<QString>*> i(*delta);
    QPair<QString, QString> pair;
    while (i.hasNext())
    {
        i.next();
        pair = i.key();
        printf("key< %s %s >, pair", pair.first.toStdString().c_str(), pair.second.toStdString().c_str());
        QSet<QString>* value = i.value();
        debugPrintSet(value);
        printf("\n");
    }
}

void Nfa::debugPrintSet(QSet<QString>* set)
{
    QSetIterator<QString> j(*set);
    printf("{ ");
    while (j.hasNext())
    {
        printf("%s ", j.next().toStdString().c_str());
    }
    printf("}");
}

bool Nfa::isValidString(QString string, bool isParallel)
{
    // Save the set from running runNfa into a variable so that it can
    // be properly deleted.
    QSet<QString>* finalStates;
    if (isParallel)
    {
        if (VERBOSE)
        {
            printf("Traversing NFA in parallel.\n");
        }
        finalStates = runNfaP(string);
    }
    else
    {
        if (VERBOSE)
        {
            printf("Traversing NFA sequentially.\n");
        }
        finalStates = runNfa(string);
    }
    bool intersects = finalStates->intersect(*f).count() > 0;
    delete finalStates; // Remove memory allocated for finalStates.
    return intersects;
}

*/

QSet<Node>* Nfa::runNfa(QString string)
{
  //Traversal* trav = new Traversal(0, q0);
  //QSet<Node>* travs = 
  //travs.insert(trav);
  return traverse(q0, &string, FORWARDS);
}


QSet<Node>* Nfa::traverse(Node* node, QString* str, int direction)
{
    QSet<Node>* qSet = node->rawStates(direction);
    QPair<Node, QString> pair;

    /* NOTE: No longer necessary when using rawStates
    // Special Case: Check to see if epsilon jump exists between initial state and another state
    QPair<Node, QString> pair;
    pair.first = *state;
    pair.second = QString("@");

    // If there's an epsilon jump to any other states, jump
    if (delta->contains(pair))
    {
        qSet->unite(*delta->value(pair));
    }
    */

    // Represents all the states that you ARE GOING TO be in, during the next iteration
    QSet<Node>* newSet = new QSet<Node>();;
    for (int i = 0; i < str->size(); i++)
    {
        // Empty the new states (current states are kept in qSet)
        newSet->clear();
        QSetIterator<Node> j(*qSet);
        while(j.hasNext())
        {
            // Create a pair of State and transition
            /*
            pair.first = j.next();
            pair.second = (*str)[i];
            // If the State-Transition pair has any matches, add them to the new set
            if (delta->contains(pair))
            {
                newSet->unite(*delta->value(pair));
            }
            */
            QString subStr((*str).at(i));
            newSet->unite(*(((Node)j.next()).traverseOn(subStr, direction)));

            /*
            // Check for epsilon jumps, add them to the new set
            pair.second = "@";
            if (delta->contains(pair))
            {
                newSet->unite(*delta->value(pair));
            }
            */
            // newSet becomes current set (qSet) for the next iteration

            // If all the transitions leave the NFA, no matches, you're done
        }

        QSetIterator<Node> k(*qSet);
        printf("uniting:\n");
        while(k.hasNext())
          printf("\t%s\n", ((Node)k.next()).getName().toStdString().c_str());

        QSetIterator<Node> l(*newSet);
        printf("with:\n");
        while(l.hasNext())
          printf("\t%s\n", ((Node)l.next()).getName().toStdString().c_str());
        
        qSet->clear();
        qSet->unite(*newSet); // Done this way for memory management.

        if (qSet->count() == 0)
        {
            return qSet;
        }
            
        
    }

    // Congrats! There were matches!
    delete newSet;
    return qSet;
}

/*QSet<QString>* Nfa::runNfaP(QString string)
{
    // debugPrintDelta();
    QSet<QString>* qSet = new QSet<QString>();
    qSet->insert(q0);

    QPair<QString, QString> pair;
    pair.first = q0;
    pair.second = QString("@");
    // printf("key< %s %s >\n", pair.first.toStdString().c_str(), pair.second.toStdString().c_str());

    if (delta->contains(pair))
    {
        qSet->unite(*delta->value(pair));
        //debugPrintSet(qSet);
        //printf("\n");
    }

    QSet<QString>* newSet = new QSet<QString>();;
    #pragma omp parallel for
    for (int i = 0; i < string.length(); i++)
    {
        newSet->clear();
        QSetIterator<QString> j(*qSet);
        while(j.hasNext())
        {
            pair.first = j.next();
            pair.second = string[i];
            if (delta->contains(pair))
            {
                newSet->unite(*delta->value(pair));
            }

            pair.second = QString("@");
            if (delta->contains(pair))
            {
                newSet->unite(*delta->value(pair));
            }

            qSet->clear();
            qSet->unite(*newSet); // Done this way for memory management.

            //if (qSet->count() == 0)
            //{
            //    return qSet;
            //}
        }
    }

    //debugPrintSet(qSet);
    //printf("\n");
    return qSet;
}*/
