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
class Bookmark : public Saveable{
    VideoProject* m_vid_proj;
    int m_type = UNSORTED;
    std::string m_container_name = "";
    std::string m_file;
    int frame_number = -1;       // Frame at which the bookmark was taken
    int time = -1;               // Time of the bookmark (in millisecs)
    std::string description = "";    // Description for the bookmark, given by user
public:
    Bookmark(VideoProject* vid_proj, const std::string& text, const int& frame_nbr);
    Bookmark();
    int get_time();
    int get_frame_number();
    int get_type();
    VideoProject* get_video_project();
    std::string get_container_name();
    std::string get_description();
    void set_description(const std::string &text);
    void set_type(const int type);
    void set_container_name(const std::string name);
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    Q_DECL_DEPRECATED void export_frame();
    Q_DECL_DEPRECATED void create_file_path();
    Q_DECL_DEPRECATED void remove_exported_image();

private:

};

#endif // BOOKMARK_H
