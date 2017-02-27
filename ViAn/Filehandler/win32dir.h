#ifndef WIN32DIR_H
#define WIN32DIR_H
#include <string>
#include <windows.h>
#include "stringhelper.h"
int makeDir(std::string dirpath);
int removeDir(std::string dirpath);
#endif // WIN32DIR_H

