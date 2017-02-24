#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <vector>
#include <string>
#include <windows.h>
#include <iostream>
#include <fstream>
#include "project.h"
#include "stringhelper.h"
#include <conio.h>

#define WORKSPACE "C:/Programmering"
typedef int ID;
struct VideoProj
{
    //struct VideoFile file;
    //std::vector<Analysis> analyses;
};

class Project;
struct POIFile{

};

struct Bookmark
{
    double timeStamp; //in seconds
    double frame;   //what frame is saved
};

class FileHandler
{
public:
    FileHandler();
    std::string workSpace;

    Project* openProject(std::string dirpath);
    Project* createProject(std::string projName);
    Project* getProject(ID id);
    void saveProject(Project* proj);
    Project* loadProject(std::string filePath);
    void addVideo(Project* proj, std::string filePath);
    void extract_proj_obj(std::string line);
    int deleteProject(Project* proj);

    ID createDirectory(std::string path);
    int deleteDirectory(std::string dirpath);
    ID createFile(std::string filename, ID dirID);
    int deleteFile(ID id);
    void writeFile(ID id, std::string text);
    void readFile(ID id, size_t linesToRead, std::string& buf);

    std::string getDir(ID id);

    std::string getFile(ID id);    

//    saveAnalysis
//    saveDrawing
//    exportToDoc
//    makeImage
//    writeComment
//    generateReport
private:
    // pid 0 => returns first element
    std::map<ID,Project*> m_projects;
    std::map<ID, std::string> m_fileMap;
    std::map<ID, std::string> m_dirMap;
    ID m_pid;
    ID m_fid;
    ID m_did;

};

#endif // FILEHANDLER_H
