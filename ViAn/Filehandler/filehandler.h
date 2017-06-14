#ifndef FILEHANDLER_H
#define FILEHANDLER_H
// I/O
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
//  STD
#include <mutex>
#include <vector>
#include <algorithm>
#include <memory>
// QT
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QJsonDocument>

// ViAn files
#include "Project/project.h"
#include "saveable.h"

enum WRITE_OPTION{APPEND, OVERWRITE};

typedef int ID;       // ID, defined for code readability.

class Project; // fix for include issue

class FileHandler : public Saveable{
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


    ID file_id;                 //counter for file ids
    ID dir_id;                  //counter for directory ids

};

#endif // FILEHANDLER_H
