%{
#include "parse.h"
%}

%x VAL
%option 8bit batch noyywrap nounput noinput never-interactive
%%

<VAL>{
[^ \t\n][^\n]* { BEGIN(INITIAL); printf("value %s\n", yytext); yylval.str = strdup(yytext); return VALUE; }
}

<*>[ \t]*             { /* ignore whitespace */ }
<*>\n                 { /* eat newlines      */ }
[a-zA-Z0-9_+-]+       { printf("id %s\n", yytext); yylval.str = strdup(yytext); return ID; }
;.*                   { /* ignore comments */ }
=                     { BEGIN(VAL); return '='; }
.                     { return (int)yytext[0]; }
%%
