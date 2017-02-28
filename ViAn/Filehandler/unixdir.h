#ifndef UNIXDIR_H
#define UNIXDIR_H
#include <unistd.h>
#include <string>
int makeDir(std::string dirpath);
int removeDir(std::string dirpath);

#endif // UNIXDIR_H
