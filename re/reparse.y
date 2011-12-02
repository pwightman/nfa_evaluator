%{
#include <stdio.h>
#include <string.h>

  void yyerror(const char *str)
  {
    fprintf(stderr,"error: %s\n",str);
  }

  int yywrap()
  {
    return 1;
  }

  main()
    {
      yyparse();
    }

  %}

%token t_PLUS t_STAR t_LPAREN t_RPAREN t_EPS t_STRING

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
  printf("%d + %d",$1, $3);
}
;

