#include "dir.h"
/**
 * @todo make threadsafe
 * @brief make_dir
 * create a directory from given path.
 * OBS! Only compatible with windows systems
 * @param std::string
 */
int make_dir(std::string dirpath){
     CreateDirectory(s2ws(dirpath).c_str(),NULL);
     return GetLastError();
}
/**
 * @todo make threadsafe
 * @brief remove_dir
 * @param std::string
 * Delete a direcetory from given path, will not delete
 * non empty directories.
 * OBS! Only compatible with windows systems.
 */
 int remove_dir(std::string dirpath){
     RemoveDirectory(s2ws(dirpath).c_str());
     return GetLastError();
 }
