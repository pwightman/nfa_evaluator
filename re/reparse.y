%{
#include <stdio.h>
#include <string.h>

#define YYSTYPE char *

extern int yydebug;

//ifdef _cplusplus
 extern "C"
 {
   int yyparse(void);
   int yylex(void);
   int yywrap()
   {
     return 1;
   }
 } 
//endif
  
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
   char* str;
 }

%token t_PLUS t_STAR t_LPAREN t_RPAREN t_EPS
%token <str> t_STRING
%%

commands:
| commands command
;


command:
p_expression_plus
;

p_expression_plus:
t_STRING t_PLUS t_STRING
{
  printf("%s + %s",$1, $3);
}
;

