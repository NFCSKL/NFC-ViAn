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
#include "video.h"

enum BOOKMARK_TYPE {UNSORTED = 3010, DISPUTED, REFERENCE};
using ID = int;
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
    std::pair<int, std::string> m_container;

    QString m_time = "";               // Time of the bookmark (format "mm:ss")
    std::string m_description = "";    // Description for the bookmark, given by user
    VideoState m_state;                 // Contains the state of the video, eg frame, zoom rect and scale factor

    bool m_unsaved_changes = true;  // Track whether the class instance has unsaved changes
    ID id = 0;
public:
    std::string m_file;
    Bookmark(VideoProject* vid_proj, const std::string file_name, const std::string& text, const VideoState& state, const QString m_time);
    Bookmark(const Bookmark& bookmark);
    Bookmark();
    ~Bookmark();
    ID get_id();
    void set_id(ID id);
    void reset_root_dir(const std::string& dir);
    QString get_time();
    int get_frame_number();
    VideoState get_state();
    std::pair<int, std::string> get_container();
    VideoProject* get_video_project();
    std::string get_description();
    void set_description(const std::string &text);
    void set_container(std::string name, int type);
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
