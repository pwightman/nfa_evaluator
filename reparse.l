%{
#include <stdio.h>
#include "reparse.hpp"

//[\w^\d]\w*
// [a-zA-Z_][a-zA-Z0-9_]* {yylval.string=strdup(yytext); return t_STRING;}
%}

%% Tokens 

[a-zA-Z0-9]            { yylval.symbol=strdup(yytext); return t_SYMBOL; }
[ \t]+                    /* Ignore whitespace */;
.                      return (int) yytext[0];

%%
