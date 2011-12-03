#include <QtCore/QCoreApplication>
#include <QHash>
#include <QSet>
#include <QTime>
#include "stdio.h"

#include "nfa.h"
#include "node.h"

void debugPrintSet(QSet<Node>* set)
{
    QSetIterator<Node> i(*set);

    printf("[ ");
    while(i.hasNext())
    {
        Node node = i.next(); // Not doing it this way caused a weird const error.
        printf("%s ", node.getName().toStdString().c_str());
    }
    printf("]\n");
}

static int qHash(const Node& node)
{
    return (int)(long)&node;
}

void hash_testing()
{
    QSet<Node> set;
    Node* testingNode = new Node();

    int i;
    
    printf("contains before add: %s\n", (set.contains(*testingNode) ? "Yes" : "No"));
    set.insert(*testingNode);
    printf("contains after add: %s\n", (set.contains(*testingNode) ? "Yes" : "No"));
    printf("\n---\n\n");
}

int main()
{

    // Nfa* nfa = new Nfa();
    Node* s1 = new Node("s1");
    Node* s2 = new Node("s2");
    Node* s3 = new Node("s3");
    Node* s4 = new Node("s4");
    Node* s5 = new Node("s4");
    Node* s6 = new Node("s6");
    s1->addRelation(*s3, "1");
    s1->addRelation(*s4, "@");
    s1->addRelation(*s6, "2");
    s6->addRelation(*s2, "0");
    s6->addRelation(*s2, "1");
    s4->addRelation(*s2, "3");
    s3->addRelation(*s2, "4");
    s3->addRelation(*s4, "@");
    s2->addRelation(*s1, "4");
    s2->addRelation(*s5, "1");
    s5->addRelation(*s1, "2");
    s5->addRelation(*s3, "2");
    s3->addRelation(*s5, "2");

    if (VERBOSE)
    {
        s1->debugPrint();
        s3->debugPrint();
        s4->debugPrint();
        s6->debugPrint();
        s2->debugPrint();

        // Raw state tests...

        printf("Raw states of s1: ");
        debugPrintSet(s1->rawStates(FORWARDS));

        printf("Backwards raw states of s4: ");
        debugPrintSet(s4->rawStates(BACKWARDS));

        // Traversal tests...

        printf("Forward traversal set of node s5 on 2: ");
        debugPrintSet(s5->traverseOn("2", FORWARDS));

        printf("Backward traversal set of node s5 on 2: ");
        debugPrintSet(s5->traverseOn("2", BACKWARDS));

        printf("Forward traversal set of node s5 on 6: ");
        debugPrintSet(s5->traverseOn("6", FORWARDS));
    }

    // Use the same NFA as described above, but change the state names to be able
    // to continue testing.

    printf("Working\n");

    Node* s11 = new Node("s1");
    Node* s12 = new Node("s2");
    Node* s13 = new Node("s3");
    Node* s14 = new Node("s4");
    Node* s15 = new Node("s4");
    Node* s16 = new Node("s6");

    Nfa nfa;
    nfa.addTransition(*s11, *s13, "1");
    nfa.addTransition(*s11, *s14, "@");
    nfa.addTransition(*s11, *s16, "2");
    nfa.addTransition(*s16, *s12, "0");
    nfa.addTransition(*s16, *s12, "1");
    nfa.addTransition(*s14, *s12, "3");
    nfa.addTransition(*s13, *s12, "4");
    nfa.addTransition(*s13, *s14, "@");
    nfa.addTransition(*s12, *s11, "4");
    nfa.addTransition(*s12, *s15, "1");
    nfa.addTransition(*s15, *s11, "2");
    nfa.addTransition(*s15, *s13, "2");
    nfa.addTransition(*s13, *s15, "2");
    nfa.makeInitial(*s11);

    /* This is where it's crashing. For some reason, when it was:
         Nfa* nfa = new Nfa();

       nfa.f has an address in the constructor, but is NULL inside of makeFinal.

       I changed it to
         Nfa nfa;

       and now nfa.f has an adress, but it's different than the one in the constructor.
       Which probably just means it's garbage. Bleargh...
    */
    nfa.makeFinal(*s12);
    nfa.makeFinal(*s14);

    // Completely new Nfa representing (a+b)b
    /*Node* s21 = new Node("s1");
    Node* s22 = new Node("s2");
    Node* s23 = new Node("s3");
    Node* s24 = new Node("s4");

    Nfa* nfa1 = new Nfa();
    nfa1->addTransition(*s21, *s22, "a");

    Nfa* nfa2 = new Nfa();
    nfa2->addTransition(*s23, *s24, "b");

    // Union of a + b
    nfa1->unite(*nfa2);

    // Not quite clear how a concatenation would work here...
    Node* s25 = new Node("s5");
    Node* s26 = new Node("s6");
    Nfa* nfa3 = new Nfa();
    nfa3->addTransition(*s25, *s26, "b");

    nfa1->concatenate(*nfa3);

    // At this point Nfa1 should be (a+b)b...*/
}
