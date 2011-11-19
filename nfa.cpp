#include "nfa.h"
#include "stdio.h"
#include <QHashIterator>

Nfa::Nfa(QSet<QString>* Q, QString q0, QSet<QString>* sigma, QHash<QPair<QString, QString>*, QSet<QString>*>* delta, QSet<QString>* f)
{
    this->Q = Q;
    this->q0 = q0;
    this->sigma = sigma;
    this->delta = delta;
    this->f = f;
}

Nfa* Nfa::createNfa(QSet<QString>* Q, QString q0, QSet<QString>* sigma, QHash<QPair<QString, QString>*, QSet<QString>*>* delta, QSet<QString>* f)
{
    QHashIterator<QPair<QString, QString>*, QSet<QString>*> i(*delta);
    QPair<QString, QString>* pair;
    while (i.hasNext())
    {
        i.next();
        pair = i.key();
        printf("key< %s %s >, pair{ ", pair->first.toStdString().c_str(), pair->second.toStdString().c_str());
        QSet<QString>* value = i.value();
        QSetIterator<QString> j(*value);
        while (j.hasNext())
        {
            printf("%s ", j.next().toStdString().c_str());
        }
        printf("}\n");
    }

    return new Nfa(Q, q0, sigma, delta, f);
}

QSet<QString>* Nfa::runNfa()
{
    QSet<QString>* qSet = new QSet<QString>();
    qSet->insert(q0);

    QPair<QString, QString>* pair = new QPair<QString, QString>();
    pair->first = q0;
    pair->second = QString("@");
    // printf("%s\n", delta

    if (delta->contains(pair))
    {
        printf("Got here");
    }

    // Stub
    return NULL;
}

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
