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
#include <algorithm>
#ifdef _WIN32
    #define WORKSPACE "C:"
#elif __APPLE__
    #define WORKSPACE "/Applications"
#elif __unix__
    #define  WORKSPACE "~"

#endif


enum WRITE_OPTION{APPEND, OVERWRITE};
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
    Project* open_project(std::string dirpath);
    Project* create_project(std::string projName);
    FH_ERROR delete_project(Project* proj);
    Project* load_project(std::string fullProjectPath);
    Project* load_project(std::string projname, std::string dirpath);
    void save_project(ID id);
    void save_project(Project* proj);


    void add_video(Project* proj, std::string filePath);
    //directory manipulation
    //varying implementation
    ID create_directory(std::string dirpath);
    FH_ERROR delete_directory(ID id);

    //file manipulation

    ID create_file(std::string filename, ID dirID);
    FH_ERROR delete_file(ID id);
    void write_file(ID id, std::string text, WRITE_OPTION opt = WRITE_OPTION::APPEND);
    void read_file(ID id,  std::string& buf, int linesToRead = -1);




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
    ID load_project_file(std::string filePath, std::stringstream& projFileStream);
    void load_proj_files(std::string str);
    //add used for loading project from file
    void add_file(ID id ,std::string filepath);
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
