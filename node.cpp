#include "node.h"
#include "stdio.h"

extern void printSet(QSet<Node*>* set);

Node::Node()
{
    connections = new QHash<QString, QSet<Node*>*>();
    reverseConnections = new QHash<QString, QSet<Node*>*>();
}

Node::Node(QString name)
{
    connections = new QHash<QString, QSet<Node*>*>();
    reverseConnections = new QHash<QString, QSet<Node*>*>();
    this->name = name;
}

QString Node::getName()
{
    return name;
}

QHash<QString, QSet<Node*>*>* Node::getHash(int direction)
{
    QHash<QString, QSet<Node*>*>* links;
    if (direction == FORWARDS)
    {
        links = connections;
    }
    else if (direction == BACKWARDS)
    {
        links = reverseConnections;
    }
    else
    {
        if (VERBOSE)
        {
            printf("Incorrect direction passed into traversOn\n");
        }
        return NULL;
    }
    return links;
}

/*
def eclosure(NFA, Q):
  """ E-closure(NFA, Q) obtains, given a starting set of states Q, the set of states                                                                                                                       
  reachable by the NFA by traversing zero or more epsilon labeled transitions                                                                                                                              
  """
  return ech(NFA, Q)
def ech(NFA, Allsofar, Previous=set()):
  """ Extend Allsofar until nothing new (that's not in Previous) """
  if (Allsofar == Previous):
    return Allsofar
  else:
    all_state_sets_one_eps_away = list(map(lambda state: step_nfa(NFA, state, ''), Allsofar))
    all_state_sets_one_eps_away = reduce(lambda x,y: set(x) | set(y),\
                                  all_state_sets_one_eps_away + [set()]) # base case                                                                                                                        
    return ech(NFA, set(all_state_sets_one_eps_away) | set(Allsofar), Allsofar) 
 */

QSet<Node*>* Node::rawStates(int direction)
{
/*
    QHash<QString, QSet<Node*>*>* links = getHash(direction);
    if (links == NULL)
    {
        // If an incorrect direction was passed in, then return an empty set.
        if (VERBOSE)
        {
            printf("Incorrect direction passed into rawStates\n");
        }
        return new QSet<Node*>();
    }

    // Create a set with this node.
    QSet<Node*>* states = new QSet<Node*>();
    states->insert(this);

    // If there are epsilon jumps, add them to the set.
    if (links->contains("@"))
    {
        states->unite(*links->value("@"));
    }

    // Return the set.
*/
  
    QSet<Node*>* nodes = new QSet<Node*>();
    nodes->insert(this);
    return eClosure(nodes, NULL, direction);
}

/*
def ech(NFA, Allsofar, Previous=set()):
  """ Extend Allsofar until nothing new (that's not in Previous) """
  if (Allsofar == Previous):
    return Allsofar
  else:
    all_state_sets_one_eps_away = list(map(lambda state: step_nfa(NFA, state, ''), Allsofar))
    all_state_sets_one_eps_away = reduce(lambda x,y: set(x) | set(y),\
                                  all_state_sets_one_eps_away + [set()]) # base case                                                                                                                        
    return ech(NFA, set(all_state_sets_one_eps_away) | set(Allsofar), Allsofar) 
 */


QSet<Node*>* Node::eClosure(QSet<Node*>* allSoFar, QSet<Node*>* previous, int direction)
{
  
  if(previous == NULL)
  {
    previous = new QSet<Node*>();
  }

  /* Base case */
  if ((*allSoFar) == (*previous)) 
  {
    return allSoFar;
  }

  if(VERBOSE)
  {
    printf("\n\n==============\n\n");
    printf("All so far:\n");
    printSet(allSoFar);
    printf("\n");
  }

  QSet<Node*>* one_step_eps = new QSet<Node*>();

  QMutableSetIterator<Node*> i(*allSoFar);
  while(i.hasNext())
  {
    one_step_eps->unite(*i.next()->connections->value("@"));    
  }

  one_step_eps->unite(*allSoFar);

  if(VERBOSE)
  {
    printf("After closure:\n");
    printSet(one_step_eps);
    printf("\n");
  }

  return eClosure(one_step_eps, allSoFar, direction);
}

QSet<Node*>* Node::traverseOn(QString str, int direction)
{
  if(VERBOSE)
    printf("traversing on %s:\n", str.toStdString().c_str());
    /* If they're checking for epsilon, just return yourself */
    if (str == "@")
    {
      QSet<Node*>* set = new QSet<Node*>();
      set->insert(this);
      return set;
    }

    QHash<QString, QSet<Node*>*>* links = getHash(direction);

    if (links == NULL)
    {
        // If an incorrect direction was passed in, then return an empty set.
        if (VERBOSE)
        {
            printf("Incorrect direction passed into traverseOn\n");
        }
        return NULL;
    }

    // Create an empty set.
    QSet<Node*>* states = new QSet<Node*>();

    states->insert(this);

    // If there are traversals on the value, then add them to the set.
    if (links->contains(str))
    {
        states->unite(*links->value(str));
        QMutableSetIterator<Node*> i(*states);
        while(i.hasNext())
          states->unite(*(i.next()->rawStates(direction)));
    }

    // Return the set.
    return states;
}

void Node::addRelation(Node& destination, QString value)
{
    if (VERBOSE && name != "")
    {
        printf("Checking to see if %s is in the hash of node %s.\n", value.toStdString().c_str(), destination.getName().toStdString().c_str());
    }
    // Add forward traversal relation.
    if (connections->contains(value))
    {
        QSet<Node*>* set = connections->value(value);
        set->insert(&destination);
    }
    else
    {
        if (VERBOSE && name != "")
        {
            printf("%s wasn't found in the hash, so creating a new entry.\n", value.toStdString().c_str());
        }
        QSet<Node*>* set = new QSet<Node*>();
        set->insert(&destination);
        connections->insert(value, set);
    }

    // Add backward traversal relation.
    if (destination.reverseConnections->contains(value))
    {
        QSet<Node*>* set = destination.reverseConnections->value(value);
        set->insert(this);
    }
    else
    {
        QSet<Node*>* set = new QSet<Node*>();
        set->insert(this);
        destination.reverseConnections->insert(value, set);
    }
    if (VERBOSE && name != "")
    {
        printf("---\n");
    }
}

void Node::debugPrint()
{
    if (name == "")
    {
        printf("Not printing debug information because 'name' wasn't set.\n");
        return;
    }

    QHashIterator<QString, QSet<Node*>*> i(*connections);
    while(i.hasNext())
    {
        i.next();
        QString string = i.key();
        QSet<Node*>* set = i.value();
        printf("%s FORWARD ON %s -> [ ", this->name.toStdString().c_str(), string.toStdString().c_str());

        QSetIterator<Node*> j(*set);
        while (j.hasNext())
        {
            printf("%s ", j.next()->name.toStdString().c_str());
        }
        printf("]\n");
    }

    QHashIterator<QString, QSet<Node*>*> i2(*reverseConnections);
    while(i2.hasNext())
    {
        i2.next();
        QString string2 = i2.key();
        QSet<Node*>* set2 = i2.value();
        printf("%s BACKWARD ON %s -> [ ", this->name.toStdString().c_str(), string2.toStdString().c_str());

        QSetIterator<Node*> j2(*set2);
        while (j2.hasNext())
        {
            printf("%s ", j2.next()->name.toStdString().c_str());
        }
        printf("]\n");
    }
}
/*
static int qHash(const Node& node)
{
    return (int)(long)&node;
}
*/

bool Node::operator==(const Node &other) const
{
    return this == &other;
}
