%{
#include <stdio.h>
#include <string.h>

  //#define YYSTYPE char *

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
   char* string;
 }

%token <string> t_STRING
%%

commands:
| commands command
;


command:
p_expression_plus
;

p_expression_plus:
t_STRING '+' t_STRING
{
  printf("%s + %s",$1,$3);
}
;

