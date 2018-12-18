#ifndef BOOKMARK_H
#define BOOKMARK_H

#include "Filehandler/saveable.h"
#include "video.h"

#include <QJsonObject>

enum BOOKMARK_TYPE {UNSORTED, DISPUTED, REFERENCE};

/**
 * @brief The Bookmark class
 * Bookmark class is used for storing bookmarks, i.e. user
 * marked points in a video and an associated frame.
 */
class Project;
class VideoProject;
class Bookmark : public Writeable {
    friend class BookmarkTest;
    Project* m_proj = nullptr;
    int m_vid_proj_id;

    QString m_time = "";               // Time of the bookmark (format "mm:ss")
    QString m_description = "";    // Description for the bookmark, given by user
    VideoState m_state;                 // Contains the state of the video, eg frame, zoom rect and scale factor
    QString m_thumbnail_path = "";        // Contains the path to the thumbnail for the bookmark
    int m_index;                            // Contains the bookmarks index in the qlistwidget, updates when saved
    int m_type = UNSORTED;
    int m_container_id = -1;

    bool m_unsaved_changes = true;  // Track whether the class instance has unsaved changes
public:
    QString m_file;             // Path for the frame in the bookmark
    QString m_image_name;
    Bookmark(const QString &file_name, const QString& text,
             const VideoState& state, const QString &m_time,
             const int& type = UNSORTED);
    Bookmark(const Bookmark& bookmark);
    Bookmark();
    ~Bookmark();
    int get_vid_proj_id();
    void set_vid_proj_id(int new_id);
    int get_index();
    void set_index(int index);
    int get_type();
    void reset_root_dir(const QString &dir);
    QString get_time();
    int get_frame_number();
    VideoState get_state();
    int get_container_id();
    Project* get_project();
    QString get_description();
    void set_description(const QString &text);
    void set_container(int new_id, int type);
    void set_thumbnail_path(QString path);
    QString get_thumbnail_path();
    void set_project(Project* proj);
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    bool is_saved() const;
    void remove_exported_image();

};

#endif // BOOKMARK_H
