#include <stdio.h>
#include <QString>
#include <QVector>
#include <QHash>
#include <QSet>
#include <iostream>
#include "nfa.h"

/* Please update this as you add tests */
int NUM_TESTS = 1;

/* Used to specify granularity of testing */
typedef enum {
  TestTypeSequential,
  TestTypeParallel,
  TestTypeBoth
} TestType;

/* Declaration for the assert method defined below */
void assert_nfa(Nfa* nfa, QString str, bool expected, TestType type);

static const char* debugString(QSet<QString>* states)
{
  QList<QString> list = states->toList();
  QString str = "{ ";
  int len = list.size();
  for(int i = 0; i < len; i++)
  {
    str += list[i];
    if(i != len - 1)
      str += ", ";
  }

  str += " }";

  return str.toStdString().c_str();
}
/* A nice, pretty string about which type of test was run */
const char* testTypeStr(TestType type)
{
  return (type == TestTypeSequential ? "Sequential" : "Parallel");
}
/*
 * Defines the NFA: even 1's and 0's (?)
 */

Nfa* nfa_1()
{
  Nfa* nfa = new Nfa();
  Node* s1 = new Node("s1");
  Node* s2 = new Node("s2");
  nfa->addTransition(*s1, *s2, "1");
  nfa->makeInitial(*s1);
  nfa->makeFinal(*s2);

  return nfa;
}

/* Testing on nfa_1 */
void test_nfa_1(TestType type)
{
  Nfa* nfa = nfa_1();
  assert_nfa(nfa, "0101", true, type);
  assert_nfa(nfa, "01010", false, type);
}

/*
 * Does the busy-work of assertion so testing inside of test_nfa_X can 
 * be nice and clean
 */
void assert_nfa(Nfa* nfa, QString str, bool expected, TestType type)
{
  /* A result is made up of the result (bool) and what type was run (TestType) */
  QVector<QPair<bool,TestType> > results;

  /* Run the tests for the correct type */
  if((type == TestTypeSequential) || (type == TestTypeBoth))
    results.push_back(
      QPair<bool,TestType>(
        (nfa->isValidString(str, false) == expected), TestTypeSequential
      )
    );
   
  if((type == TestTypeParallel) || (type == TestTypeBoth))
    results.push_back(
      QPair<bool,TestType>(
        (nfa->isValidString(str, true) == expected), TestTypeParallel
      )
    );

  /* Assert the true/false results of all the tests that were run */
  for(int i = 0; i < results.size(); i++)
  {
    if(results[i].first)
      printf("PASSED: %s, %s\n", str.toStdString().c_str(), testTypeStr(results[i].second));
    else
    {
      printf("FAILED: %s\n", str.toStdString().c_str());
      printf("\tExpected: %s\n", (expected ? "true" : "false"));
      printf("\tReceived: %s\n", (expected ? "false" : "true"));
      printf("\tType: %s\n", testTypeStr(results[i].second));
    }
  }

  /* When testing both, this is a more robust test of making sure the returned sets were equal */
  /*
  if (type == TestTypeBoth) 
  {
    QSet<Node>* first = nfa->runNfa(str);
    QSet<Node>* second = nfa->runNfaP(str);
    if((*first) != (*second))
    {
      printf("FAILED: %s\n", str.toStdString().c_str());
      printf("\tParallel and Sequential result sets do not match:");
      printf("\tSequential : %s\n", debugString(first));
      printf("\tParallel   : %s\n", debugString(second));
    }
  }
  */
  
}

/*
 * Switches on the NFA test of your choice
 */
void test_nfa(int num, TestType type)
{
  switch(num)
  {
    case 1:
      test_nfa_1(type);
      break;
  }
}

/*
 * Runs all the tests
 */
void test_all(TestType type)
{
  for(int i = 1; i < NUM_TESTS + 1; i++)
  {
    test_nfa(i, type);
  }
}

/*
 * Runs the tests
 */
int main()
{
  test_all(TestTypeSequential);
}

