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
    Project* loadProject(std::string filePath);

    void addVideo(Project* proj, std::string filePath);

    //directory manipulation
    //varying implementation
    ID createDirectory(std::string dirpath);
    FH_ERROR deleteDirectory(std::string dirpath);

    //file manipulation
    ID createFile(std::string filename, ID dirID);
    FH_ERROR deleteFile(ID id);
    void writeFile(ID id, std::string text);
    void readFile(ID id, size_t linesToRead, std::string& buf);

    // thread safe read operations for maps
    std::string getDir(ID id);
    Project* getProject(ID id);
    std::string getFile(ID id);    

private:
    void updateProjFiles(Project* proj); // used to update existing project files and maps
    // thread safe add operations for maps
    void addFile(std::pair<ID,std::string> pair);
    void addProject(std::pair<ID,Project*> pair);
    void addDir(std::pair<ID,std::string> pair);

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
