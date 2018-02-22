#ifndef BOOKMARK_H
#define BOOKMARK_H
#include <QJsonObject>
#include <QString>
#include <QImage>
#include <QImageWriter>
#include <QFile>
#include <string>
#include <iostream>
#include "Filehandler/saveable.h"
#include "videoproject.h"

enum BOOKMARK_TYPE {UNSORTED, DISPUTED, REFERENCE};

/**
 * @brief The Bookmark class
 * Bookmark class is used for storing bookmarks, i.e. user
 * marked points in a video and an associated frame.
 */
class VideoProject;
class Bookmark : public Writeable{
    friend class BookmarkTest;
    VideoProject* m_vid_proj;
//    int m_type = UNSORTED;
//    std::string m_container_name = "";
    std::vector<std::pair<int, std::string>> m_containers;  // Keeps track of all containers the bookmark resides in

    int frame_number = -1;       // Frame at which the bookmark was taken
    int time = -1;               // Time of the bookmark (in millisecs)
    std::string description = "";    // Description for the bookmark, given by user

    bool m_unsaved_changes = true;  // Track wether the class instance has unsaved changes
public:
    std::string m_file;
    Bookmark(VideoProject* vid_proj, const string file_name, const std::string& text, const int& frame_nbr);
    Bookmark();
    void reset_root_dir(const std::string& dir);
    int get_time();
    int get_frame_number();
    std::vector<std::pair<int, std::string>> get_containers();
    VideoProject* get_video_project();
    std::string get_description();
    void set_description(const std::string &text);
    void set_type(const int type);
    void add_container(std::string name, int type);
    void remove_container(std::string name, int type);
    void rename_container(std::string old_name, std::string new_name);
    void set_video_project(VideoProject* vid_proj);
    bool remove();
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    bool is_saved() const;
    Q_DECL_DEPRECATED void export_frame();
    Q_DECL_DEPRECATED void create_file_path();
    Q_DECL_DEPRECATED void remove_exported_image();
private:

};

#endif // BOOKMARK_H
