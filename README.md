FeOSIni v0.2 Beta
=================

Introduction
------------
FeOSIni is a free, open-source ini parser for the
[FeOS](https://github.com/fincs/FeOS "FeOS") operating system.
It supports reading and writing ini files.

Build Prerequisites
-------------------
You need the following in order to build FeOSIni:

- [FeOS and FeOS SDK](https://github.com/fincs/FeOS "FeOS")
- A working C compiler

Optional
--------
- bison
- flex

How to Build FeOSIni
--------------------
    cd $(FEOSSDK)/userlib
    git clone git://github.com/mtheall/FeOSIni.git
    cd /path/to/FeOS
    make libinstall

How to Use FeOSIni
------------------
In your C/C++ source:

    #include <ini.h>

In your Makefile:

    CONF_LIBDIRS = $(FEOSSDK)/userlib/FeOSIni
    CONF_LIBS = -lini

API
---
| Function                 | Description                      |
|:-------------------------|:---------------------------------|
| `Ini IniAlloc();`        | Allocate an empty ini structure. |
| `void IniFree(Ini ini);` | Deallocate an ini structure.     |
| `const char* IniGetValue(Ini ini, const char *section, const char *property);`            | Return the value for a property in a section. If the section or property does not exist, return NULL. |
| `int IniSetValue(Ini ini, const char *section, const char *property, const char *value);` | Set the value for a property in a section. If the section or property does not exist, they are created. Returns 0 for success, -1 for failure. |
| `Ini IniRead(const char *filename);`           | Read an ini file and return a structure filled in with the file's contents. Returns NULL for failure. |
| `int IniWrite(Ini ini, const char *filename);` | Write an ini file with the contents of the ini structure. Returns 0 for succes, -1 for failure.       |

