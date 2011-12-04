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
    //yydebug=0;
    yyparse();
  }

%}

%union
 {
   char* symbol;
   char* nfa;
 }

/* Bison Declarations */
%token<symbol> t_SYMBOL
%type<symbol> re term factor

%left '+'  /* union */
%left '*'  /* kleene star */

/* Grammar Follows */
%%

commands:
| commands re
;

re:       re '+' term    { $$=strcat($1,strcat((char*)"+",$3)); }
        | term 
;

term:     factor
        | term factor    {$$ = strcat($1,$2);}
          
;

factor:   t_SYMBOL
        | '(' re ')'     { $$ = strcat((char*)"(",strcat($2,(char*)")")); }
        | factor '*'     { $$ = strcat($1,(char*)"*"); }
        | '@'            { $$ = (char*)"*"; }         
;

%%
