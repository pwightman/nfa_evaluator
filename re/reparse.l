%{
#include <stdio.h>
#include "y.tab.h"
%}

%% Tokens 
\+ return t_PLUS;
\* return t_STAR;
\( return t_LPAREN;
\) return t_RPAREN;
\" return t_EPS;
[a-zA-Z_][a-zA-z0-9_]* return t_STRING;
[ \t]+ /* Ignore whitespace */;
%%