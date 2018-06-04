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

    int m_frame_nbr = -1;           // Frame at which the bookmark was taken
    QString m_time = "";               // Time of the bookmark (format "mm:ss")
    std::string m_description = "";    // Description for the bookmark, given by user

    bool m_unsaved_changes = true;  // Track whether the class instance has unsaved changes
public:
    std::string m_file;
    Bookmark(VideoProject* vid_proj, const std::string file_name, const std::string& text, const int& frame_nbr, const QString m_time);
    Bookmark(const Bookmark& bookmark);
    Bookmark();
    void reset_root_dir(const std::string& dir);
    QString get_time();
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
    void add_to_video_project();
    bool remove();
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    bool is_saved() const;
    void remove_exported_image();
private:

};

#endif // BOOKMARK_H
