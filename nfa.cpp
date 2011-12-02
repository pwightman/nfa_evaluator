#include "nfa.h"
#include "stdio.h"
#include <QHashIterator>

Nfa::Nfa()
{
    Q = new QSet<QString>();
    q0 = "";
    sigma = new QSet<QString>();
    delta = new QHash<QPair<QString, QString>, QSet<QString>*>();
    reverseDelta =  new QHash<QPair<QString, QString>, QSet<QString>*>();
    f = new QSet<QString>();
}

void Nfa::addTransition(QString source, QString destination, QString path)
{

}

void Nfa::debugPrintDelta()
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

QSet<QString>* Nfa::runNfa(QString string)
{
  Traversal* trav = new Traversal(0, &q0);
  return traverse(trav, &string);
}

QSet<QString>* Nfa::traverse(Traversal* trav, QString* str)
{
/*

Pseudo-code for interative nfa algorithm
using this we should be able to parallelize using openMP

accepts_string(str)
 return {run_nfa}.intersect(F) != empty_set

run_nfa(str)
 q_set = {}
 q_set.add(q0)
 if ( (q0,eps) in delta_keys)
   q_set.add(delta[q0,eps])

 new_set = {}
 for (int i = 0; i < str.length; i++)
   new_set = {};
   for q in q_set
     if (q,str[i]) in delta_keys)
       new_set.add(delta[q,string[i])
     if (q,eps) in delta_keys
       new_set.add(delta[q,eps])
   q_set = new_set
   if (q_set == empty)
     return empty
  return q_set

  */
    // debugPrintDelta();
    QSet<QString>* qSet = new QSet<QString>();
    qSet->insert(*(trav->state()));

    QPair<QString, QString> pair;
    pair.first = *(trav->state());
    pair.second = QString("@");
    // printf("key< %s %s >\n", pair.first.toStdString().c_str(), pair.second.toStdString().c_str());

    if (delta->contains(pair))
    {
        qSet->unite(*delta->value(pair));
        /*debugPrintSet(qSet);
        printf("\n");*/
    }

    QSet<QString>* newSet = new QSet<QString>();;
    for (int i = 0; i < str->size(); i++)
    {
        newSet->clear();
        QSetIterator<QString> j(*qSet);
        {
            pair.first = j.next();
            pair.second = (*str)[i];
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

            if (qSet->count() == 0)
            {
                return qSet;
            }
        }
    }

    /*debugPrintSet(qSet);
    printf("\n");*/
    // printf("%d\n", qSet->intersect(*f).count() > 0);
    delete newSet;
    return qSet;

}

QList<Traversal*>* Nfa::step(Traversal* trav, QString* str)
{
  return new QList<Traversal*>();
}

QSet<QString>* Nfa::runNfaP(QString string)
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
}

