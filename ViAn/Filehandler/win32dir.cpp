#include "dir.h"
/**
 * @todo make threadsafe
 * @brief make_dir
 * create a directory from given path.
 * OBS! Only compatible with windows systems
 * @param std::string
 */
int make_dir(std::string dirpath){
     if(!CreateDirectory(s2ws(dirpath).c_str(),NULL)){//if function fails return val is false
         return GetLastError();
     }else{
         return 0;
     }
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
     if(!RemoveDirectory(s2ws(dirpath).c_str())){ //if function fails return val is false
         return GetLastError();
     }else{
         return 0;
     }
 }
