#include "nfa.h"
#include <algorithm>
#include "stdio.h"
#include <QHashIterator>

#define NUM_THREADS 4 // DO NOT SET LESS THAN 2!


Nfa::Nfa()
{
    q0 = new Node();
    f = new QSet<Node*>();
}

Nfa* Nfa::simple(QString str)
{
  Node* s1 = new Node();
  Node* s2 = new Node();
  Nfa* nfa = new Nfa();
  nfa->addTransition(*s1, *s2, str);
  nfa->makeInitial(*s1);
  nfa->makeFinal(*s2);

  return nfa;
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
    f->insert(&node);
}

void Nfa::unite(Nfa& nfa)
{
    Node* newq0 = new Node("AG"); // AG: Auto generated.
    newq0->addRelation(*q0, "@");
    newq0->addRelation(*nfa.q0, "@");
    /* combine the final states of both nfa's into this nfa */
    f->unite(*nfa.f);
    makeInitial(*newq0);
}

void Nfa::concatenate(Nfa& nfa)
{
    // Make all the final states transition to nfa.q0
    QSetIterator<Node*> i(*f);
    while(i.hasNext())
    {
        Node* node = i.next();
        node->addRelation(*nfa.q0, "@");
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

    // Link all final states to newq0.
    QSetIterator<Node*> i(*f);
    while (i.hasNext())
    {
        Node* node = i.next();
        node->addRelation(*newq0, "@");
    }

    // Make newq0 the new q0
    makeInitial(*newq0);
    // Accept epsilon
    makeFinal(*newq0);
}

/*
static int qHash(const Node& node)
{
    return (int)(long)&node;
}
*/

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
*/

void printSet(QSet<Node*>* set)
{
    QSetIterator<Node*> i(*set);
    Node* node;
    while(i.hasNext())
    {
        node = i.next();
        node->debugPrint();
    }
}

bool Nfa::isValidString(QString string, bool isParallel)
{
    // Save the set from running runNfa into a variable so that it can
    // be properly deleted.
    /*
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
    */

    return (isParallel ? runNfaP(string) : runNfa(string));
}


bool Nfa::runNfa(QString string)
{
  QSet<Node*>* set = new QSet<Node*>();
  set->insert(q0);
  /* Sending -1 to traverse implies sequential execution */
  QSet<Node*>* endingStates = traverse(set, &string, -1);
  endingStates->intersect(*f);
  bool intersects = endingStates->count() > 0;
  delete endingStates; // Remove memory allocated for finalStates.
  
  return intersects;
}


QSet<Node*>* Nfa::traverse(QSet<Node*>* node, QString* str, int direction)
{
    QSet<Node*>* q = node; //node->rawStates(direction);

    /* 
     * We want the rest of the functions to perform only considering the
     * Possibility of FORWARDS/BACKWARDS, not -1
     */
    int trav_direction = (direction == -1 || direction == FORWARDS ? FORWARDS : BACKWARDS);

    // Epsilon closure
    QSetIterator<Node*> k(*q);
    while (k.hasNext())
    {
        Node* node = k.next();
        q->unite(*node->rawStates(trav_direction));
    }

    /*
     * Setup variables that loop will use
     */
    int incr = 1;
    int limit = str->size();
    int i = 0;
    if(direction != -1)
    {
      if(direction == FORWARDS)
      {
        limit = (str->size() % 2 == 0 ? str->size() / 2 + 1 : str->size() / 2 );
        if(VERBOSE)
          printf("FORWARDS: i: %d, limit: %d\n", i, limit);//(direction == FORWARDS ? "Forwards" : "Backwards"));
      }
      /* Backwards */
      else
      {
        limit = str->size() / 2 + 1;
        incr = -1;
        i = str->size() - 1;
        if(VERBOSE)
          printf("BACKWARDS: i: %d, limit: %d, string length: %d\n", i, limit, str->size());//(direction == FORWARDS ? "Forwards" : "Backwards"));
      }
    }

    //printf("Outside: i: %d, limit: %d\n", i, limit);//(direction == FORWARDS ? "Forwards" : "Backwards"));

    QSet<Node*>* newSet = new QSet<Node*>();
    /* You want to loop backwards sometimes, so the comparison you do depends on that */
    for (; ( trav_direction == FORWARDS ? i < limit : i >= limit ); i += incr)
    {
    //printf("i: %d, limit: %d\n", i, limit);//(direction == FORWARDS ? "Forwards" : "Backwards"));
        newSet->clear();
        QSetIterator<Node*> j(*q);
        while (j.hasNext())
        {
            Node* node = j.next();
            QString subStr(str->at(i));
            newSet->unite(*node->traverseOn(subStr, trav_direction));
        }

        // Make q eqaul newSet.
        q->clear();
        q->unite(*newSet); // Done this way for mem. management.

        // Epsilon closure.
        QSetIterator<Node*> k(*q);
        while (k.hasNext())
        {
            Node* node = k.next();
            q->unite(*node->rawStates(trav_direction));
        }

        // Break out early if the q set ends up empty.
        if (q->count() == 0)
        {
            return q;
        }
    }
    return q;
}

/*
typedef struct 
{
  QList<QSet<Node*>*>* results
} NfaParams;
*/

QList<QSet<Node*>*>* Nfa::partition()
{
  QList<QSet<Node*>*>* sets = new QList<QSet<Node*>*>();

  int i;
  int num_finals = f->size();

  /*
   * Create sets for each of the threads, except one, which will
   * be dedicated to the initial state
   */
  for (i = 0; i < std::min(NUM_THREADS - 1, num_finals); i++) 
  {
    QSet<Node*>* set = new QSet<Node*>();
    sets->append(set);
  }

  QMutableSetIterator<Node*> j(*f);
  i = 0;
  while(j.hasNext())
  {
    /* Round robin all of the final states into the sets */
    (*sets)[i % (NUM_THREADS - 1)]->insert(j.next());
    i++;
  }

  // Add the inital to its own set
  // IMPORTANT: runNfaP depends on this being the last element in the list
  QSet<Node*>* initialSet = new QSet<Node*>();
  initialSet->insert(q0);
  sets->append(initialSet);

  return sets;
}

typedef struct
{
  Nfa* nfa;
  QSet<Node*>* nodes;
  QString* str;
  bool isInitial;
} NfaParams;

void* traverseP(void* _params)
{
  NfaParams* params = (NfaParams*)_params;

  int direction = (params->isInitial ? FORWARDS : BACKWARDS);
  QSet<Node*>* results = params->nfa->traverse(params->nodes, params->str, direction);

  return _params;
}

bool Nfa::runNfaP(QString string)
{
  /* Create the partition of states */
  QList<QSet<Node*>*>* part = partition();

  /* Allocate threads depending on size partitioning */
  pthread_t* threads = (pthread_t*)malloc(part->size()*sizeof(pthread_t));
  /* Allocate params for each thread */
  NfaParams* params = (NfaParams*)malloc(part->size()*sizeof(NfaParams));
  /* Initial state set will be stored here later in the code */
  QSet<Node*>* initialStateSet = NULL;
  /* Loop counter */
  int i;

  /*
   *  IMPORTANT: An assumption is being made here that the very last set 
   *  in the list contains ONLY the inital state
   *
   */
  for(i = 0; i < part->size(); i++)
  {
    params[i].nfa = this;
    params[i].str = &string;
    /* If the set is the initial state, mark it as such */
    if (i == part->size() - 1) 
    {
      initialStateSet = part->at(i);
      params[i].isInitial = true;
    }
    else
    {
      params[i].isInitial = false;
    }
    /* Assign the params being sent to the thread the list it will be working on */
    params[i].nodes = part->at(i);
    /* Send off the thread! */
    pthread_create(&threads[i], NULL, &traverseP, &params[i]);
  }

  /* Reap the threads */
  for (i = 0; i < part->size(); i++) 
  {
    int* ptr;
    pthread_join(threads[i], (void**)&ptr);
  }

  bool intersects = false;

  /* Again, we're assuming the last list is the inital state so we aren't checking it */
  /* If there's any intersection between the initial set and the final sets then it was a valid string */
  for(i = 0; i < part->size() - 1; i++)
  {
    if(initialStateSet->intersect(*part->at(i)).size() > 0)
      intersects = true;
  }

  free(threads);
  free(params);

  return intersects;
}
