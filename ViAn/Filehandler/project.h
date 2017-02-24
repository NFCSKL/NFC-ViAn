x#ifndef PROJECT_H
#define PROJECT_H
#include <map>
#include <string>
#include <cstddef>
#include <windows.h>
#include <iostream>
#include <fstream>
#include "stringhelper.h"
#include "filehandler.h"
typedef int Analysis;
typedef int ID;
typedef size_t Drawing;

struct Video{
    ID id;
    std::vector<Analysis> analyses;
    std::vector<Drawing> drawings;   
    Video(ID id){
        this->id = id;
    }
};

class Project{
public:
    Project(ID id, std::string name);
    Project(std::string dirpath);
    ~Project();
    void addVideo(Video vid);
    void open();
    void close();
    void remove();
private:
    ID m_vid;    
public:
    ID m_id;
    std::string m_name;
    ID m_dir;
    ID m_file;
    std::vector<Video> m_videos;
    //http://codereview.stackexchange.com/questions/38879/parsing-text-file-in-c
    //2017-02-24
    friend std::istream& operator>>(std::istream& str, Project* proj)
    {
        std::string line;
        Project tmp;
        std::string str ;
        std::string type;
        if(std::getline(str, line))
        {
            std::stringstream iss(line);
            std::getline(iss, type, ":");
            switch(type){
            case "N": //ProjName
                std::getline(iss, tmp->name, ':'); //get project name
            case "V":
                std::getline(iss, str, ':'); //get video filepath
                proj->addVideo(str);
            case "A":
                //add analysis to project
                //proj->addAnalysis();
            case "D":
                // add drawing to project
                //proj->addDrawing();

            }
        }
    }
    void swap(Person& other) throws() // C++03 as this answer was written a long time ago.
    {
        swap(name,        other.name);
        swap(age,         other.age);
        swap(salary,      other.salary);
        swap(hoursWorked, other.hoursWorked);
        swap(randomText,  other.randomText)
    }
};
#endif // PROJECT_H
