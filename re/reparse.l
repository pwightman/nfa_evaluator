%{
#include <stdio.h>
#include "reparse.hpp"

#define YYSTYPE char *
//[\w^\d]\w*
%}

%% Tokens 
\+ return t_PLUS;
\* return t_STAR;
\( return t_LPAREN;
\) return t_RPAREN;
\" return t_EPS;
[a-zA-Z_][a-zA-Z0-9_]* {yylval=strdup(yytext); return t_STRING;}
\s+ /* Ignore whitespace */;
%%