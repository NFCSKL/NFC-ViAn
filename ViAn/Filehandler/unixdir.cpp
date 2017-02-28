#include "unixdir.h"


/**
 * @todo make threadsafe
 * @brief makeDir
 * create a directory from given path.
 * OBS! Only compatible with unix based systems
 * @param std::string
 */
int makeDir(std::string dirpath){
    const int dir_err = mkdir(dirpath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err)
    {
        printf("Error creating directory!n");
        exit(1);
    }
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

}
