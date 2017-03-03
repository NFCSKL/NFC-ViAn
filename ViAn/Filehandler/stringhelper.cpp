#include "stringhelper.h"

//
// Turn string into wide string
// source : http://codereview.stackexchange.com/questions/419/converting-between-stdwstring-and-stdstring
// date 28/02/2017
std::wstring s2ws(const std::string& str)
{
    int len;
    int slength = (int)str.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}
