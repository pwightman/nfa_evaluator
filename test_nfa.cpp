#include <stdio.h>
#include <QString>
#include <QVector>
#include <QHash>
#include <QSet>
#include <iostream>
#include "nfa.h"


/* Please update this as you add tests */
int NUM_TESTS = 12;
extern void printSet(QSet<Node*>* set);

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

/* Forward declarations */
Nfa* nfa_1();
Nfa* nfa_2();
Nfa* nfa_3();
Nfa* nfa_4();
Nfa* nfa_5();
Nfa* nfa_6();
Nfa* nfa_7();
Nfa* nfa_10();
Nfa* nfa_11();
Nfa* nfa_12();
/* End forward declarations */

/* combination test
   (01+11)*1+01+110*1
*/
Nfa* nfa_12()
{
  Nfa* nfa = new Nfa();
  Node* s1 = new Node("s1");
  Node* s2 = new Node("s2");
  Node* s3 = new Node("s3");
  nfa->addTransition(*s1,*s2,"0");
  nfa->addTransition(*s2,*s3,"1");
  nfa->makeInitial(*s1);
  nfa->makeFinal(*s3); // (01) 
  
  Nfa* nfa11 = new Nfa();
  Node* s4 = new Node("s4");
  Node* s5 = new Node("s5");
  Node* s6 = new Node("s6");
  nfa11->addTransition(*s4,*s5,"1");
  nfa11->addTransition(*s5,*s6,"1");
  nfa11->makeInitial(*s4);
  nfa11->makeFinal(*s6);

  Nfa* nfa1 = Nfa::simple("1");
  
  nfa->unite(*nfa11); // (01+11)
  nfa->star(); // (01+11)*
  nfa->concatenate(*nfa1); // (01+11)*1  

  Nfa* nfa01b = new Nfa();
  Node* s7 = new Node("s7");
  Node* s8 = new Node("s8");
  Node* s9 = new Node("s9");
  nfa01b->addTransition(*s7,*s8,"0");
  nfa01b->addTransition(*s8,*s9,"1");
  nfa01b->makeInitial(*s7);
  nfa01b->makeFinal(*s9);

  nfa->unite(*nfa01b); // (01+11)*1+01

  Nfa* nfa_last = new Nfa();
  Node* s10 = new Node("s10");
  Node* s11 = new Node("s11");
  nfa_last->addTransition(*s10,*s11,"1");
  nfa_last->makeInitial(*s10);
  nfa_last->makeFinal(*s11); // (1)
  
  Nfa* nfa1b = Nfa::simple("1");
  nfa_last->concatenate(*nfa1b); // (11)

  Nfa* nfa0 = Nfa::simple("0");
  nfa0->star(); // (0)*
  nfa_last->concatenate(*nfa0); // (110*)
  
  Nfa* nfa1c = Nfa::simple("1");
  nfa_last->concatenate(*nfa1c); // (110*1)

  nfa->unite(*nfa_last); // (01+11)*1+01+110*1

  return nfa;
}

/*  (01+11)*1+01+110*1 */
void test_nfa_12(TestType type)
{
  Nfa* nfa = nfa_12();

  // assert_nfa(nfa, "@",       true,  type);
  assert_nfa(nfa, "",        false, type);
  assert_nfa(nfa, "011",     true,  type);
  assert_nfa(nfa, "111",     true,  type);
  assert_nfa(nfa, "01011",   true,  type);
  assert_nfa(nfa, "11111",   true,  type);
  assert_nfa(nfa, "0111011", true,  type);
  assert_nfa(nfa, "1",       true,  type);
  assert_nfa(nfa, "01",      true,  type);
  assert_nfa(nfa, "11",      false, type);
  assert_nfa(nfa, "111",     true,  type);
  assert_nfa(nfa, "1101",    true,  type);
  assert_nfa(nfa, "1100001", true,  type);
  assert_nfa(nfa, "0111",    false, type);
  assert_nfa(nfa, "0101",    false, type);
  assert_nfa(nfa, "1111",    false, type);

}

/* lots of epsilon jumps test */
Nfa* nfa_11()
{
  Nfa* nfa = new Nfa();
  Node* s1 = new Node("s1");
  Node* s2 = new Node("s2");
  Node* s3 = new Node("s3");
  Node* s4 = new Node("s4");
  Node* s5 = new Node("s5");
  Node* s6 = new Node("s6");
  Node* s7 = new Node("s7");
  Node* s8 = new Node("s8");
  Node* s9 = new Node("s9");
  Node* s10 = new Node("s10");

  nfa->addTransition(*s1,*s2, "@");
  nfa->addTransition(*s2,*s3, "a");
  nfa->addTransition(*s3,*s4, "@");
  nfa->addTransition(*s4,*s5, "b");
  nfa->addTransition(*s5,*s6, "@");
  nfa->addTransition(*s6,*s7, "@");
  nfa->addTransition(*s7,*s8, "c");
  nfa->addTransition(*s8,*s9, "@");
  nfa->addTransition(*s9,*s10,"d");

  nfa->makeInitial(*s1);
  nfa->makeFinal(*s10);

  return nfa;
}

void test_nfa_11(TestType type)
{
  Nfa* nfa = nfa_11();

  assert_nfa(nfa, "", false, type);
  // assert_nfa(nfa, "@", false, type);
  assert_nfa(nfa, "a", false, type);
  assert_nfa(nfa, "ab", false, type);
  assert_nfa(nfa, "ac", false, type);
  assert_nfa(nfa, "abcd", true, type);
  assert_nfa(nfa, "aabcd", false, type);
  assert_nfa(nfa, "abcda", false, type);
  assert_nfa(nfa, "abcad", false, type);
}

/* union star test 
   (0+1)*
*/
Nfa* nfa_10()
{
  Nfa* nfa1 = new Nfa();
  Node* s1 = new Node("s1");
  Node* s2 = new Node("s2");
  nfa1->addTransition(*s1, *s2, "0");
  nfa1->makeInitial(*s1);
  nfa1->makeFinal(*s2);

  Nfa* nfa2 = new Nfa();
  Node* s3 = new Node("s3");
  Node* s4 = new Node("s4");
  nfa2->addTransition(*s3, *s4, "1");
  nfa2->makeInitial(*s3);
  nfa2->makeFinal(*s4);

  nfa1->unite(*nfa2);
  nfa1->star();
  
return nfa1;
}

void test_nfa_10(TestType type)
{
  Nfa* nfa = nfa_10();
  // assert_nfa(nfa, "@",          true,  type);
  assert_nfa(nfa, "1",          true,  type);
  assert_nfa(nfa, "0",          true,  type);
  assert_nfa(nfa, "01",         true,  type);
  assert_nfa(nfa, "0101",       true,  type);
  assert_nfa(nfa, "1110",       true,  type);
  assert_nfa(nfa, "010101",     true,  type);
  assert_nfa(nfa, "0101010101", true,  type);
  assert_nfa(nfa, "0101010201", false, type);
}

/* cat star test 
   (01)*
*/
Nfa* nfa_9()
{
  Nfa* nfa1 = new Nfa();
  Node* s1 = new Node("s1");
  Node* s2 = new Node("s2");

  Nfa* nfa2 = new Nfa();
  Node* s3 = new Node("s3");
  Node* s4 = new Node("s4");

  nfa1->addTransition(*s1, *s2, "0");
  nfa1->makeInitial(*s1);
  nfa1->makeFinal(*s2);

  nfa2->addTransition(*s3, *s4, "1");
  nfa2->makeInitial(*s3);
  nfa2->makeFinal(*s4);

  nfa1->concatenate(*nfa2);
  nfa1->star();
  
return nfa1;
}

void test_nfa_9(TestType type)
{
  Nfa* nfa = nfa_9();
  // assert_nfa(nfa, "@",          true,  type);
  assert_nfa(nfa, "1",          false, type);
  assert_nfa(nfa, "0",          false, type);
  assert_nfa(nfa, "01",         true,  type);
  assert_nfa(nfa, "0101",       true,  type);
  assert_nfa(nfa, "1110",       false, type);
  assert_nfa(nfa, "010101",     true,  type);
  assert_nfa(nfa, "0101010101", true,  type);
}

/* cat test 
   (01)
*/
Nfa* nfa_8()
{
  Nfa* nfa1 = new Nfa();
  Node* s1 = new Node("s1");
  Node* s2 = new Node("s2");

  Nfa* nfa2 = new Nfa();
  Node* s3 = new Node("s3");
  Node* s4 = new Node("s4");

  nfa1->addTransition(*s1, *s2, "0");
  nfa1->makeInitial(*s1);
  nfa1->makeFinal(*s2);

  nfa2->addTransition(*s3, *s4, "1");
  nfa2->makeInitial(*s3);
  nfa2->makeFinal(*s4);

  nfa1->concatenate(*nfa2);

  return nfa1;
}

void test_nfa_8(TestType type)
{
  Nfa* nfa = nfa_8();
  // assert_nfa(nfa, "@",          false, type);
  assert_nfa(nfa, "1",          false, type);
  assert_nfa(nfa, "0",          false, type);
  assert_nfa(nfa, "01",         true,  type);
  assert_nfa(nfa, "0101",       false, type);
  assert_nfa(nfa, "1110",       false, type);
  assert_nfa(nfa, "010101",     false, type);
  assert_nfa(nfa, "0101010101", false, type);
}

/* star test 
   1*
*/
Nfa* nfa_7()
{
  Nfa* nfa = new Nfa();
  Node* s1 = new Node("s1");
  Node* s2 = new Node("s2");

  nfa->addTransition(*s1, *s2, "1");
  nfa->makeInitial(*s1);
  nfa->makeFinal(*s2);

  nfa->star();

  return nfa;
}

void test_nfa_7(TestType type)
{
  Nfa* nfa = nfa_7();
  // assert_nfa(nfa, "@",      true,  type);
  assert_nfa(nfa, "1",      true,  type);
  assert_nfa(nfa, "0",      false, type);
  assert_nfa(nfa, "11",     true,  type);
  assert_nfa(nfa, "111",    true,  type);
  assert_nfa(nfa, "1110",   false, type);
  assert_nfa(nfa, "11111",  true,  type);
  assert_nfa(nfa, "111111", true,  type);
}

/* star multi test 
   (01)*
*/
Nfa* nfa_6()
{
  Nfa* nfa = new Nfa();
  Node* s1 = new Node("s1");
  Node* s2 = new Node("s2");
  Node* s3 = new Node("s3");

  nfa->addTransition(*s1, *s2, "0");
  nfa->addTransition(*s2, *s3, "1");
  nfa->makeInitial(*s1);
  nfa->makeFinal(*s3);

  nfa->star();

  return nfa;
}

void test_nfa_6(TestType type)
{
  Nfa* nfa = nfa_6();
  // assert_nfa(nfa, "@",          true,  type);
  assert_nfa(nfa, "1",          false, type);
  assert_nfa(nfa, "0",          false, type);
  assert_nfa(nfa, "01",         true,  type);
  assert_nfa(nfa, "0101",       true,  type);
  assert_nfa(nfa, "010101",     true,  type);
  assert_nfa(nfa, "01010101",   true,  type);
  assert_nfa(nfa, "0101010101", true,  type);
}

/*
 * Same as test_nfa_3 but tests the simple method
 */
Nfa* nfa_5()
{
  Nfa* nfa1 = Nfa::simple("0");

  Nfa* nfa2 = Nfa::simple("1");

  nfa1->unite(*nfa2);

  return nfa1;
}

void test_nfa_5(TestType type)
{
  Nfa* nfa = nfa_5();
  // assert_nfa(nfa, "@", false, type);
  assert_nfa(nfa, "1", true, type);
  assert_nfa(nfa, "0", true, type);
  assert_nfa(nfa, "2", false, type);
  assert_nfa(nfa, "00", false, type);
  assert_nfa(nfa, "10", false, type);
  assert_nfa(nfa, "01", false, type);
  assert_nfa(nfa, "11", false, type);
}

/*
 * uses nfa_2
 */
void test_nfa_4(TestType type)
{
  qsrand(NULL);
  Nfa* nfa = nfa_2();  
  QString str = "0";

  for(int i = 0; i < 1000000; i++)
    str += (rand() % 2 == 1 ? "1" : "0");

  printf("\nBegin stress test...\n");
  assert_nfa(nfa, str, true, type);
}

/*
 * (1 + 0)
 */
Nfa* nfa_3()
{
  Nfa* nfa1 = new Nfa();
  Node* s1 = new Node("s1");
  Node* s2 = new Node("s2");

  nfa1->addTransition(*s1, *s2, "0");
  nfa1->makeInitial(*s1);
  nfa1->makeFinal(*s2);

  Nfa* nfa2 = new Nfa();
  Node* s3 = new Node("s3");
  Node* s4 = new Node("s4");
  nfa2->addTransition(*s3, *s4, "1");
  nfa2->makeInitial(*s3);
  nfa2->makeFinal(*s4);

  nfa1->unite(*nfa2);

  return nfa1;
}

void test_nfa_3(TestType type)
{
  Nfa* nfa = nfa_3();
  // assert_nfa(nfa, "@", false, type);
  assert_nfa(nfa, "1", true, type);
  assert_nfa(nfa, "0", true, type);
  assert_nfa(nfa, "2", false, type);
  assert_nfa(nfa, "00", false, type);
  assert_nfa(nfa, "10", false, type);
  assert_nfa(nfa, "01", false, type);
  assert_nfa(nfa, "11", false, type);
}
/*
 * (0 + 1)*
 */
Nfa* nfa_2()
{
  Nfa* nfa = new Nfa();
  Node* s1 = new Node("s1");
  Node* s2 = new Node("s2");
  Node* s3 = new Node("s3");

  nfa->addTransition(*s1, *s2, "0");
  nfa->addTransition(*s1, *s3, "1");
  nfa->addTransition(*s2, *s1, "@");
  nfa->addTransition(*s3, *s1, "@");
  nfa->makeInitial(*s1);
  nfa->makeFinal(*s1);
  //nfa->makeFinal(*s2);
  //nfa->makeFinal(*s3);

  return nfa;
}

void test_nfa_2(TestType type)
{
  Nfa* nfa = nfa_2();
  assert_nfa(nfa, "1", true, type);
  assert_nfa(nfa, "0", true, type);
  assert_nfa(nfa, "10", true, type);
  assert_nfa(nfa, "01", true, type);
  assert_nfa(nfa, "11110", true, type);
  assert_nfa(nfa, "00111010100011", true, type);
  assert_nfa(nfa, "00111210100011", false, type);
  assert_nfa(nfa, "2", false, type);
  // assert_nfa(nfa, "@", true, type);
}

/* one more 1 than 0's */
Nfa* nfa_1()
{
  Nfa* nfa = new Nfa();
  Node* s1 = new Node("s1");
  Node* s2 = new Node("s2");
  nfa->addTransition(*s1, *s2, "1");
  nfa->addTransition(*s2, *s1, "0");
  nfa->makeInitial(*s1);
  nfa->makeFinal(*s2);

  return nfa;
}

/* Testing on nfa_1 */
void test_nfa_1(TestType type)
{
  Nfa* nfa = nfa_1();
  assert_nfa(nfa, "1", true, type);
  assert_nfa(nfa, "10", false, type);
  assert_nfa(nfa, "11", false, type);
  assert_nfa(nfa, "101", true, type);
  assert_nfa(nfa, "101010101010101010101", true, type);
  assert_nfa(nfa, "101010101011101010101", false, type);
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
    results.append(
      QPair<bool,TestType>(
        (nfa->isValidString(str, false) == expected), TestTypeSequential
      )
    );
   
  if((type == TestTypeParallel) || (type == TestTypeBoth))
    results.append(
      QPair<bool,TestType>(
        (nfa->isValidString(str, true) == expected), TestTypeParallel
      )
    );

  /* Assert the true/false results of all the tests that were run */
  for(int i = 0; i < results.size(); i++)
  {
    if(results[i].first)
    {
      //printf("PASSED: %s, %s\n", str.toStdString().c_str(), testTypeStr(results[i].second));
      printf(".");
    }
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
  printf("TESTING NFA %d\n=====================\n\n", num);
  switch(num)
  {
    case 1:
      test_nfa_1(type);
      break;
    case 2:
      test_nfa_2(type);
      break;
    case 3:
      test_nfa_3(type);
      break;
    case 4:
      //test_nfa_4(type);
      break;
    case 5:
      test_nfa_5(type);
      break;
    case 6:
      test_nfa_6(type);
      break;
    case 7:
      test_nfa_7(type);
      break;
    case 8:
      test_nfa_8(type);
      break;
    case 9:
      test_nfa_9(type);
      break;
    case 10:
      test_nfa_10(type);
      break;
    case 11:
      test_nfa_11(type);
      break;
    case 12:
      test_nfa_12(type);
      break;
    default:
      return;
}
  printf("\n\n");
}

void assert_raw(Node* node, QSet<Node*>* expected)
{
  QSet<Node*>* actual;
  if(*( actual = node->rawStates(FORWARDS)) == *expected)
    printf(".");
  else
  {
    printf("FAILED:\n");
    printf("\tActual  :\n");
    printSet(actual);
    printf("\tExpected:\n");
    printSet(expected);
  }
  
  
}

void test_raw_states()
{
  Node* s1 = new Node("s1");
  Node* s2 = new Node("s2");

  QSet<Node*>* test1 = new QSet<Node*>();
  test1->insert(s1);

  assert_raw(s1, test1);

  s1->addRelation(*s2, "@");

  test1->insert(s2);
  assert_raw(s1, test1);

  Node* s3 = new Node("s3");
  test1->insert(s3);
  s2->addRelation(*s3,"@");
  s3->addRelation(*s1,"@");

  assert_raw(s1,test1);
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
  // test_nfa(11, TestTypeSequential);
  //test_raw_states();
  printf("\n");
}

