#ifndef INI_H
#define INI_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void* Ini;

Ini         IniAlloc();
void        IniFree(Ini ini);
const char* IniGetValue(Ini ini, const char *section, const char *property);
int         IniSetValue(Ini ini, const char *section, const char *property, const char *value);
Ini         IniRead(const char *filename);
int         IniWrite(Ini ini, const char *filename);

#ifdef __cplusplus
}
#endif

#endif /* INI_H */

