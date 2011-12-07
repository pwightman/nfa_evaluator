/* Regular expression to NFA parser generator -- reparse */
%{
#include <stdio.h>
#include <string.h>
#include "nfa.h"
#include <QString>

extern int yydebug;

//#ifdef _cplusplus
 extern "C"
 {
   int yyparse(void);
   int yylex(void);
   int yywrap()
   {
     return 1;
   }
 } 
 //#endif
  
void yyerror(const char *str)
{
  fprintf(stderr,"error: %s\n",str);
}

int main()
{
  yydebug=0;
  yyparse();
}

%}

%union
{
    char* symbol;
    Nfa* nfa;
}

/* Bison Declarations */
%token<symbol> t_SYMBOL
%type<nfa> re term factor

%left '+'  /* union */
%left '*'  /* kleene star */

/* Grammar Follows */
%%

commands: /* empty */
        | re
;

re:       re '+' term    { $1->unite(*$3);/*nfa_union*/ }
        | term 
;

term:   factor
        | term factor    { $1->concatenate(*$2); /*nfa_concatenate*/}
          
;

factor:   factor '*'     { $1->star(); /*nfa_star*/}
        | '(' re ')'     { $$ = $2; /*parenthesis matching*/}
        | t_SYMBOL       { $$ = Nfa::simple(QString(t_SYMBOL)); /*make simple nfa*/ }
        | '@'            { $$ = Nfa::simple("@"); } /*epsilon jump*/
;

%%
