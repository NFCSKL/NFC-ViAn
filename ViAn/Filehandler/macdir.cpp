#include "dir.h"
/*
 * OBS, doesnt actually seem to work,
 * may just be permission issue.
 */

/**
 * @todo make threadsafe
 * @brief makeDir
 * create a directory from given path.
 * OBS! Only compatible with unix based systems
 * @param std::string
 */
int make_dir(std::string dirpath){
    return mkdir(dirpath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

}
/**
 * @todo make threadsafe
 * @brief removeDir
 * @param std::string
 * Delete a direcetory from given path, will not delete
 * non empty directories.
 * OBS! Only compatible with windows systems.
 */
int remove_dir(std::string dirpath){
    return rmdir(dirpath.c_str());
}
