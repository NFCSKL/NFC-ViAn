#ifndef FILEHANDLER_H
#define FILEHANDLER_H
// OS specific
#ifdef _WIN32
    #include <windows.h>
    #include "stringhelper.h"
#endif
// I/O
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
//  STD
#include <mutex>
#include <vector>
#include <algorithm>
// QT
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QJsonDocument>

// ViAn files
#include "project.h"
#include "saveable.h"

enum WRITE_OPTION{APPEND, OVERWRITE};

typedef int ID;       // ID, defined for code readability.

class Project; // fix for include issue

class FileHandler : Saveable{
    enum SAVE_FORMAT {JSON, BINARY};    // Formats supported by save_project

public:
    friend class test_report_generator;
    FileHandler();
    ~FileHandler();
    //  Workspace methods
    ID work_space;
    void set_work_space(std::string new_work_space);
    QDir get_work_space();

    void save();
    void load();
    void read(const QJsonObject& json);
    void write(QJsonObject& json);

    //  Project methods
    //  Project* open_project(ID id); // To be added
    //  Project* close_project(ID id);
    Project* get_project(ID id);
    Project* create_project(QString proj_name, std::string dir_path, std::string vid_path);
    bool delete_project(ID proj_id);
    Project* load_project(std::string full_project_path);
    void save_project(ID id);
    void open_project(ID id);
    void close_project(ID id);


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
    std::vector<ID> open_projects;
private:    
    // File methods
    ID add_file(QString file);
    void add_file(ID id , QString file);

    // Saveable methods
    Saveable* load_saveable(Saveable *saveable, std::string full_path, SAVE_FORMAT save_form);
    bool save_saveable(Saveable* saveable, ID dir_id, FileHandler::SAVE_FORMAT save_format);

    // Project methods
    ID add_project(Project* proj);
    void add_project(ID id, Project *proj);
    Project* load_project(std::string proj_name, std::string dir_path);
    void save_project(Project* proj);

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
