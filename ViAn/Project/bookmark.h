#ifndef BOOKMARK_H
#define BOOKMARK_H

#include "Filehandler/saveable.h"
#include "video.h"

#include <QJsonObject>

enum BOOKMARK_TYPE {UNSORTED, DISPUTED, REFERENCE};
using ID = int;
/**
 * @brief The Bookmark class
 * Bookmark class is used for storing bookmarks, i.e. user
 * marked points in a video and an associated frame.
 */
class VideoProject;
class Bookmark : public Writeable {
    friend class BookmarkTest;
    VideoProject* m_vid_proj;
    std::pair<int, std::string> m_container;

    QString m_time = "";               // Time of the bookmark (format "mm:ss")
    std::string m_description = "";    // Description for the bookmark, given by user
    VideoState m_state;                 // Contains the state of the video, eg frame, zoom rect and scale factor
    std::string m_thumbnail_path = "";        // Contains the path to the thumbnail for the bookmark

    bool m_unsaved_changes = true;  // Track whether the class instance has unsaved changes
    ID id = 0;
public:
    std::string m_file;
    Bookmark(VideoProject* vid_proj, const std::string &file_name, const std::string& text, const VideoState& state, const QString &m_time);
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
    void set_thumbnail_path(std::__cxx11::string path);
    std::__cxx11::string get_thumbnail_path();
    void set_video_project(VideoProject* vid_proj);
    void add_to_video_project();
    bool remove();
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    bool is_saved() const;
    void remove_exported_image();

};

#endif // BOOKMARK_H
