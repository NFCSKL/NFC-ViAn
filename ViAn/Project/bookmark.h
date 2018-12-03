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
    std::pair<int, QString> m_container;

    QString m_time = "";               // Time of the bookmark (format "mm:ss")
    QString m_description = "";    // Description for the bookmark, given by user
    VideoState m_state;                 // Contains the state of the video, eg frame, zoom rect and scale factor
    QString m_thumbnail_path = "";        // Contains the path to the thumbnail for the bookmark

    bool m_unsaved_changes = true;  // Track whether the class instance has unsaved changes
    ID id = 0;
public:
    QString m_file;             // Path for the frame in the bookmark
    QString m_image_name;
    Bookmark(VideoProject* vid_proj, const QString &file_name, const QString& text, const VideoState& state, const QString &m_time);
    Bookmark(const Bookmark& bookmark);
    Bookmark();
    ~Bookmark();
    ID get_id();
    void set_id(ID id);
    void reset_root_dir(const QString &dir);
    QString get_time();
    int get_frame_number();
    VideoState get_state();
    std::pair<int, QString> get_container();
    VideoProject* get_video_project();
    QString get_description();
    void set_description(const QString &text);
    void set_container(QString name, int type);
    void rename_container(QString old_name, QString new_name);
    void set_thumbnail_path(QString path);
    QString get_thumbnail_path();
    void set_video_project(VideoProject* vid_proj);
    void add_to_video_project();
    bool remove();
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    bool is_saved() const;
    void remove_exported_image();

};

#endif // BOOKMARK_H
