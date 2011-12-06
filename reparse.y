/* Regular expression to NFA parser generator -- reparse */
%{
#include <stdio.h>
#include <string.h>
#include "nfa.h"

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

main()
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
%type<nfa> re term factor char

%left '+'  /* union */
%left '*'  /* kleene star */

/* Grammar Follows */
%%

commands: /* empty */
        | commands re
;

re:       re '+' term    { $$ = $1->unite(*$3);/*nfa_union*/ }
        | term 
;

term:     term factor    { $$ = $1->concatenate(*$2); /*nfa_concatenate*/}
        | factor
          
;

factor:   char '*'       { $$ = $1->star(); /*nfa_star*/}
        | char
;

char:     '(' re ')'         { $$ = $2; /*parenthesis matching*/}
        | '@'            { $$ = Nfa::simple("@"); } /*epsilon jump*/
        | t_SYMBOL       { $$ = Nfa::simple(t_SYMBOL); /*make simple nfa*/ }

%%
