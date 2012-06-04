#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ini.h"

#ifdef DEBUG
#define debug(x...) fprintf(stderr, x)
#else
#define debug(x...) ((void)0)
#endif

struct IniProperty {
  char               *name;
  char               *value;
  struct IniProperty *next;
};

struct IniSection {
  char               *name;
  struct IniProperty *props;
  struct IniSection  *next;
};

typedef struct IniProperty IniProperty;
typedef struct IniSection  IniSection;

static inline IniProperty* __IniAllocProperty(const char *name, const char *value) {
  debug("Create property %s = %s\n", name, value);
  IniProperty *p = malloc(sizeof(IniProperty));
  memset(p, 0, sizeof(IniProperty));
  if(p) {
    p->name = strdup(name);
    if(p->name == NULL) {
      free(p);
      return NULL;
    }

    p->value = strdup(value);
    if(p->value == NULL) {
      free(p->name);
      free(p);
      return NULL;
    }
  }

  return p;
}

static inline void __IniFreeProperty(IniProperty *p) {
  if(p) {
    free(p->name);
    free(p->value);
    free(p);
  }
}

static inline IniProperty* __IniGetProperty(IniSection *s, const char *property) {
  IniProperty *p;
  if(s == NULL)
    return NULL;

  p = s->props;
  while(p && strcmp(property, p->name))
    p = p->next;
  
  return p;
}

static inline IniSection* __IniAllocSection(const char *name, const char *property, const char *value) {
  debug("Creating section [%s] %s = %s\n", name, property, value);
  IniSection *s = malloc(sizeof(IniSection));
  memset(s, 0, sizeof(IniSection));
  if(s) {
    s->name = strdup(name);
    if(s->name == NULL) {
      free(s);
      return NULL;
    }

    s->props = __IniAllocProperty(property, value);
    if(s->props == NULL) {
      free(s->name);
      free(s);
      return NULL;
    }
  }

  return s;
}

static inline void  __IniFreeSection(IniSection *s) {
  IniProperty *p;
  if(s) {
    free(s->name);
    p = s->props;
    while(p != NULL) {
      s->props = p->next;
      __IniFreeProperty(p);
      p = s->props;
    }
    free(s);
  }
}

static inline IniSection* __IniGetSection(IniSection *s, const char *section) {
  if(s == NULL)
    return NULL;

  while(s && strcmp(section, s->name))
    s = s->next;
  
  return s;
}

FEOS_EXPORT Ini IniAlloc() {
  void *rc = malloc(sizeof(IniSection));
  memset(rc, 0, sizeof(IniSection));
  return rc;
}

FEOS_EXPORT void IniFree(Ini ini) {
  IniSection  *sect = (IniSection*)ini;
  void        *tmp;

  while(sect != NULL) {
    /* free this section */
    tmp = sect;
    sect = sect->next;
    __IniFreeSection(tmp);
  }
}

FEOS_EXPORT const char* IniGetValue(Ini ini, const char *section, const char *property) {
  IniSection  *s = (IniSection*)ini;
  IniProperty *p;

  if(s == NULL || section == NULL || property == NULL)
    return NULL;

  /* special case: initialized but no entries */
  if(s->name == NULL)
    return NULL;

  /* find section */
  s = __IniGetSection(s, section);
  if(s == NULL)
    return NULL;

  /* find property */
  p = __IniGetProperty(s, property);
  if(p == NULL)
    return NULL;

  return p->value;
}

FEOS_EXPORT int IniSetValue(Ini ini, const char *section, const char *property, const char *value) {
  IniSection  *s = (IniSection*)ini;
  IniProperty *p;
  char        *tmp;

  if(s == NULL)
    return -1;

  /* special case: initialized but no entries */
  if(s->name == NULL) {
    debug("Create section [%s]\n", section);
    s->name = strdup(section);
    if(s->name == NULL)
      return -1;

    s->props = __IniAllocProperty(property, value);
    if(s->props == NULL) {
      free(s->name);
      s->name = NULL;
      return -1;
    }
    return 0;
  }

  /* find section */
  s = __IniGetSection(s, section);
  if(s == NULL) { /* not found; let's create */
    s = (IniSection*)ini;
    while(s->next != NULL)
      s = s->next;
    s->next = __IniAllocSection(section, property, value);
    if(s->next == NULL)
      return -1;
    return 0;
  }
  else
    debug("Found section [%s]\n", section);

  /* find property */
  p = __IniGetProperty(s, property);
  if(p == NULL) { /* not found; let's create */
    p = s->props;
    while(p->next != NULL)
      p = p->next;
    p->next = __IniAllocProperty(property, value);
    if(p->next == NULL)
      return -1;
    return 0;
  }
  else
    debug("Found property [%s] %s\n", section, property);

  /* update value */
  tmp = p->value;
  p->value = strdup(value);
  if(p->value == NULL) { /* failed to allocate; revert */
    p->value = tmp;
    return -1;
  }
  
  free(tmp);
  return 0;
}

FEOS_EXPORT int IniWrite(Ini ini, const char *filename) {
  IniSection  *s = (IniSection*)ini;
  IniProperty *p;

  FILE *fp = fopen(filename, "w");
  if(fp == NULL)
    return -1;

  while(s != NULL) {
    if(fprintf(fp, "[%s]\n", s->name) < 0) {
      fclose(fp);
      return -1;
    }
    p = s->props;
    while(p != NULL) {
      if(fprintf(fp, "%s = %s\n", p->name, p->value) < 0) {
        fclose(fp);
        return -1;
      }
      p = p->next;
    }
    s = s->next;
  }

  fclose(fp);
  return 0;
}

