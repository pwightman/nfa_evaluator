%{
#include <stdio.h>
#include "reparse.hpp"

//[\w^\d]\w*
%}

%% Tokens 
[a-zA-Z_][a-zA-Z0-9_]* {yylval.string=strdup(yytext); return t_STRING;}
[ \t]+                    /* Ignore whitespace */;
.                      return (int) yytext[0];
%%