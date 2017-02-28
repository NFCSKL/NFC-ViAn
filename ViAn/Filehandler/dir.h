#ifndef DIR_H
#define DIR_H

#include <string>

#ifdef _WIN32  //OS
    #include <windows.h>
    #include "stringhelper.h"
#elif __APPLE__
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <unistd.h>
#elif __linux__
    #include <sys/stat.h>
    #include <sys/types.h>
#endif
int makeDir(std::string dirpath);
int removeDir(std::string dirpath);
#endif // DIR_H

