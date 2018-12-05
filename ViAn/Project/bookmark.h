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
class Project;
class VideoProject;
class Bookmark : public Writeable {
    friend class BookmarkTest;
    VideoProject* m_vid_proj = nullptr;   // TODO need?
    Project* m_proj = nullptr;

    QString m_time = "";               // Time of the bookmark (format "mm:ss")
    QString m_description = "";    // Description for the bookmark, given by user
    VideoState m_state;                 // Contains the state of the video, eg frame, zoom rect and scale factor
    QString m_thumbnail_path = "";        // Contains the path to the thumbnail for the bookmark
    int m_index;                            // Contains the bookmarks index in the qlistwidget, updates when saved
    int m_type = UNSORTED;
    QString m_container_name = "";

    bool m_unsaved_changes = true;  // Track whether the class instance has unsaved changes
    ID id = 0;
public:
    QString m_file;             // Path for the frame in the bookmark
    QString m_image_name;
    Bookmark(VideoProject* vid_proj, const QString &file_name,
             const QString& text, const VideoState& state,
             const QString &m_time, const int& type = UNSORTED);
    Bookmark(const Bookmark& bookmark);
    Bookmark();
    ~Bookmark();
    ID get_id();
    void set_id(ID id);
    int get_index();
    void set_index(int index);
    int get_type();
    void reset_root_dir(const QString &dir);
    QString get_time();
    int get_frame_number();
    VideoState get_state();
    QString get_container_name();
    VideoProject* get_video_project();
    Project* get_project();
    QString get_description();
    void set_description(const QString &text);
    void set_container(QString name, int type);
    void rename_container(QString old_name, QString new_name);
    void set_thumbnail_path(QString path);
    QString get_thumbnail_path();
    void set_video_project(VideoProject* vid_proj);
    void set_project(Project* proj);
    bool remove();
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    bool is_saved() const;
    void remove_exported_image();

};

#endif // BOOKMARK_H
