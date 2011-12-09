#include "timing.h"
#include <QTime>
#include <QString>

Nfa* timing_nfa_2()
{
  Nfa* a = Nfa::simple("a");
  Nfa* b = Nfa::simple("b");
  a->unite(*b);
  a->star();

  return a;
}
/*
 * ((a + b)* + (c + d)* + (ef + g)(a + b))*
 */
Nfa* timing_nfa_1()
{
  Nfa* a = Nfa::simple("a");
  Nfa* b = Nfa::simple("b");
  Nfa* c = Nfa::simple("c");
  Nfa* d = Nfa::simple("d");
  Nfa* e = Nfa::simple("e");
  Nfa* f = Nfa::simple("f");
  Nfa* g = Nfa::simple("g");
  Nfa* a1 = Nfa::simple("a");
  Nfa* b1 = Nfa::simple("b");

  a->unite(*b);
  a->star();
  c->unite(*d);
  c->star();
  e->concatenate(*f);
  e->unite(*g);
  a1->unite(*b1);
  e->concatenate(*a1);
  a->unite(*c);
  a->unite(*e);
  a->star();
 
  return a;
}

void run_timing_nfa(Nfa* nfa, int iter, QString op1, QString op2)
{
  QTime timer;
  QString str("");
  for(int i = 0; i < iter; i++)
    str += (i % 2 == 0 ? op1 : op2); 
  timer.start();
  //printf("running...");
  bool seq_return = nfa->isValidString(str, false);
  int seq_time = timer.elapsed();
  timer.restart();
  bool par_return = nfa->isValidString(str, true);
  int par_time = timer.elapsed();

  printf("%d\t%d\n", seq_time, par_time);
}

void complex_nfa()
{
  printf("Complex NFA, sizes from 1...10000\n");
  run_timing_nfa(timing_nfa_1(), 1, "aacefa", "bdddgb");
  run_timing_nfa(timing_nfa_1(), 10, "aacefa", "bdddgb");
  run_timing_nfa(timing_nfa_1(), 100, "aacefa", "bdddgb");
  run_timing_nfa(timing_nfa_1(), 1000, "aacefa", "bdddgb");
  run_timing_nfa(timing_nfa_1(), 10000, "aacefa", "bdddgb");
}

void simple_nfa()
{
  printf("Simple nfa from 1...10000\n");

  run_timing_nfa(timing_nfa_2(), 1, "a", "b");
  run_timing_nfa(timing_nfa_2(), 10, "a", "b");
  run_timing_nfa(timing_nfa_2(), 100, "a", "b");
  run_timing_nfa(timing_nfa_2(), 1000, "a", "b");
  run_timing_nfa(timing_nfa_2(), 10000, "a", "b");
  
}

void run_timing()
{
  
  complex_nfa();
  simple_nfa();
  
}
