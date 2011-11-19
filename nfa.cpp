#include "nfa.h"
#include "stdio.h"

Nfa::Nfa(QSet<QString*>* Q, QString* q0, QSet<QString*>* sigma, QHash<QPair<QString*, QString*>*, QSet<QString*>*>* delta, QSet<QString*>* f)
{
    this->Q = Q;
    this->q0 = q0;
    this->sigma = sigma;
    this->delta = delta;
    this->f = f;
}

Nfa* Nfa::createNfa(QSet<QString*>* Q, QString* q0, QSet<QString*>* sigma, QHash<QPair<QString*, QString*>*, QSet<QString*>*>* delta, QSet<QString*>* f)
{
    return new Nfa(Q, q0, sigma, delta, f);
}

QSet<QString*>* Nfa::runNfa()
{
    Set<QString*>* qSet = new QSet<QString*>();
    qSet->insert(q0);

    QPair<QString*, QString*>* pair = new QPair<QString*, QString*>();
    pair->first = q0;
    pair->second = new QString("@");
    if (delta->contains(pair))
    {
        printf("Got here");
    }
}

/**

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

  **/
