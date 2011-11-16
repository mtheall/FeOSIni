%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ini.h"

extern FILE *yyin;
int yylex(void);
int yyparse (void);

void yyerror(const char *str) {
  fprintf(stderr, "error: %s\n", str);
}

int yywrap() {
  return 1;
}

struct prop {
  char *name;
  char *value;
  struct prop *next;
};
typedef struct prop prop;

static Ini ini;
static char *section = NULL;

Ini IniRead(const char *filename) {
  yyin = fopen(filename, "r");
  if(yyin)
    return NULL;

  ini = IniAlloc();
  if(ini == NULL)
    return NULL;

  yyparse();

  fclose(yyin);
  yyin = NULL;

  free(section);
  section = NULL;
  return ini;
}

%}

%token<str> ID VALUE
%type<p>    PROPERTIES PROPERTY

%union
{
  char *str;
  struct prop *p;
}

%%

SECTIONS: /* empty */
  | SECTIONS SECTION
  ;

SECTION:
  '[' ID ']' PROPERTIES
  {
    free(section);
    section = $2;
    prop *p = $4;
    prop *q;
    while(p != NULL) {
      printf("Inserting [%s] %s = %s\n", section, p->name, p->value);
      IniSetValue(ini, section, p->name, p->value);
      free(p->name);
      free(p->value);
      q = p->next;
      free(p);
      p = q;
    }
  }
  ;

PROPERTIES: /* empty */
  {
    $$ = NULL;
  }
  | PROPERTIES PROPERTY
  {
    if($$ == NULL)
      $$ = $2;
    else {
      $1->next = $2;
      $$ = $1;
    }
  }
  ;

PROPERTY:
  ID '=' VALUE
  {
    $$ = malloc(sizeof(prop));
    if($$ != NULL) {
      $$->name  = $1;
      $$->value = $3;
      $$->next  = NULL;
    }
    else {
      yyerror("failed to alloc prop");
    }
  }
  ;

