#ifndef STRINGHELPER_H
#define STRINGHELPER_H
#include <string>
#ifdef _WIN32
    #include <windows.h>
#endif
std::wstring s2ws(const std::string& str);
#endif // STRINGHELPER_H

