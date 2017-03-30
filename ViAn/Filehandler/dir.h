#ifndef DIR_H
#define DIR_H

#include <string>
#include <iostream>
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
    #include <unistd.h>
#endif

int make_dir(std::string dirpath);
int remove_dir(std::string dirpath);
#endif // DIR_H

