#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#define WINDOWS 1
#define UNIX 2
#ifdef _WIN32
    #include <windows.h>
    #include "stringhelper.h"
#elif __APPLE__
#elif __unix__
#endif
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <mutex>
#include <sstream>
#include "project.h"
#include "dir.h"

#ifdef _WIN32
    #define WORKSPACE "C:"
#elif __APPLE__
    #define WORKSPACE "/Applications"
#elif __unix__
    #define  WORKSPACE "~"

#endif



typedef int FH_ERROR; // file handler error code
typedef int ID;
struct Project; // fix for include issue
class FileHandler
{
public:
    FileHandler();
    std::string workSpace;
    //
    //  Project manipulation
    //
    Project* openProject(std::string dirpath);
    Project* createProject(std::string projName);
    FH_ERROR deleteProject(Project* proj);
    void saveProject(Project* proj);
    Project* loadProject(std::string projname, std::string dirpath);

    void add_video(Project* proj, std::string filePath);
    //directory manipulation
    //varying implementation
    ID create_directory(std::string dirpath);
    FH_ERROR delete_directory(ID id);

    //file manipulation

    ID createFile(std::string filename, ID dirID);
    FH_ERROR deleteFile(ID id);
    void writeFile(ID id, std::string text);
    void readFile(ID id,  std::string& buf, size_t linesToRead = -1);


    // thread safe read operations for maps


    std::string get_dir(ID id);
    Project* get_project(ID id);
    std::string get_file(ID id);

    // Last error
    FH_ERROR lastError;

private:
    void update_proj_files(Project* proj); // used to update existing project files and maps
    // thread safe add operations for maps
    ID add_file(std::string filepath);
    void add_project(std::pair<ID,Project*> pair);
    ID add_dir(std::string dirpath);

    /**
     * @brief m_projects, m_fileMap, m_dirMap
     * map structures for keeping track of projects, files and directories.
     */
    std::map<ID,Project*> m_projects;
    std::map<ID, std::string> m_fileMap;
    std::map<ID, std::string> m_dirMap;
    /**
     * @todo implement smarter lock mechanism to avoid overhead
     * of only 1 reader/writer at a time
     * @brief dirMapLock, fileMapLock, projMapLock
     */
    std::mutex dirMapLock; // lock for handling directory write/read
    std::mutex fileMapLock;// lock for handling file write/read
    std::mutex projMapLock;// lock for handling project write/read
    ID m_pid; //counter for project ids
    ID m_fid; //counter for file ids
    ID m_did; //counter for directory ids

};

#endif // FILEHANDLER_H
