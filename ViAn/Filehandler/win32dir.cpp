#include "dir.h"
/**
 * @todo make threadsafe
 * @brief createDirectory
 * create a directory from given path.
 * OBS! Only compatible with windows systems
 * @param std::string
 */
int makeDir(std::string dirpath){
     CreateDirectory(s2ws(dirpath).c_str(),NULL);
     return GetLastError();
}
/**
 * @todo make threadsafe
 * @brief removeDir
 * @param std::string
 * Delete a direcetory from given path, will not delete
 * non empty directories.
 * OBS! Only compatible with windows systems.
 */
 int removeDir(std::string dirpath){
     RemoveDirectory(s2ws(dirpath).c_str());
     return GetLastError();
 }
