#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#define WINDOWS 1
#define UNIX 2
#ifdef _WIN32
    #include <windows.h>
    #include "stringhelper.h"
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
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QJsonDocument>
enum WRITE_OPTION{APPEND, OVERWRITE};
typedef int FH_ERROR; // file handler error code
typedef int ID;
struct Project; // fix for include issue
struct ProjFiles;
class FileHandler
{
    enum SaveFormat {
        JSON, BINARY
    };

public:

    FileHandler();
    //  Workspace methods
    ID work_space;
    void set_work_space(std::string new_work_space);
    QDir get_work_space();


    //  Project methods
    //  Project* open_project(ID id); // To be added
    //  Project* close_project(ID id);
    Project* get_project(ID id);
    Project* create_project(QString proj_name, std::string dir_path="");
    bool delete_project(ID proj_id);

    Project* load_project(std::string full_project_path);
    Project* load_project(std::string full_path, SaveFormat save_form);
    Project* load_project(std::string proj_name, std::string dir_path);
    void save_project(ID id);
    void save_project(Project* proj);
    bool save_project(Project* proj, ID dir_id, FileHandler::SaveFormat save_format);

    bool proj_equals(Project& proj, Project& proj2);
    friend bool operator==(Project& proj, Project& proj2);

    //  Video methods
    void remove_video_from_project(ID proj_id, ID vid_id);
    ID add_video(Project* proj, std::string file_path);

    //  Directory methods
    ID create_directory(QString dir_path);
    bool delete_directory(ID id);
    bool dirs_equal(ID id, ID id2);
    QDir get_dir(ID id);

    //  File methods
    ID create_file(QString file_name, QDir dir);
    bool delete_file(ID id);
    void write_file(ID id, QString text, WRITE_OPTION opt = WRITE_OPTION::APPEND);
    void read_file(ID id, QString& buf, int lines_to_read = -1);
    bool files_equal(ID id, ID id2);
    QString get_file(ID id);

    // Last error
    bool last_error;

private:    
    // File methods
    ID add_file(QString file);
    void add_file(ID id , QString file);

    // Project methods
    ID add_project(Project* proj);
    void add_project(ID id, Project *proj);

    // Directory methods
    ID add_dir(QDir dir);
    void add_dir(ID dir_id, QDir dir);

    /**
     * @brief projects, file_map, dir_map
     * map structures for keeping track of projects, files and directories.
     */
    std::map<ID,Project*> projects;
    std::map<ID, QString> file_map;
    std::map<ID, QDir> dir_map;

    /**
     * @todo implement smarter lock mechanism to avoid overhead
     * of only 1 reader/writer at a time
     * @brief dir_map_lock, file_map_lock, proj_map_lock
     */
    std::mutex dir_map_lock;    // lock for handling directory write/read
    std::mutex file_map_lock;   // lock for handling file write/read
    std::mutex proj_map_lock;   // lock for handling project write/read
    ID project_id;              //counter for project ids
    ID file_id;                 //counter for file ids
    ID dir_id;                  //counter for directory ids

};

#endif // FILEHANDLER_H
