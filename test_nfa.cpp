#include <stdio.h>
#include <QString>
#include <QVector>
#include <QHash>
#include <QSet>
#include <iostream>
#include "Nfa.h"

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
    QSet<QString>* Q = new QSet<QString>();
    QString q0("");
    QSet<QString>* sigma = new QSet<QString>();
    QHash<QPair<QString, QString>, QSet<QString>*>* delta = new QHash<QPair<QString, QString>, QSet<QString>*>();
    QSet<QString>* f = new QSet<QString>();

    Q->insert(QString("S1"));
    Q->insert(QString("S2"));
    Q->insert(QString("S3"));

    q0 = QString("S1");

    sigma->insert(QString("0"));
    sigma->insert(QString("1"));

    f->insert(QString("S3"));

    QPair<QString, QString> pair1;
    QPair<QString, QString> pair2;
    QPair<QString, QString> pair3;
    QSet<QString>* set = new QSet<QString>();

    pair1.first = QString("S1");
    pair1.second = QString("@");
    set->insert(QString("S2"));
    set->insert(QString("S3"));

    delta->insert(pair1, set);

    pair2.first = QString("S2");
    pair2.second = QString("1");
    set = new QSet<QString>();
    set->insert(QString("S3"));

    delta->insert(pair2, set);

    pair3.first = QString("S3");
    pair3.second = QString("0");
    set = new QSet<QString>();
    set->insert(QString("S2"));

    delta->insert(pair3, set);

    return Nfa::createNfa(Q, q0, sigma, delta, f);
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
  if (type == TestTypeBoth) 
  {
    QSet<QString>* first = nfa->runNfa(str);
    QSet<QString>* second = nfa->runNfaP(str);
    if((*first) != (*second))
    {
      printf("FAILED: %s\n", str.toStdString().c_str());
      printf("\tParallel and Sequential result sets do not match:");
      printf("\tSequential : %s\n", debugString(first));
      printf("\tParallel   : %s\n", debugString(second));
    }
  }
  
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

