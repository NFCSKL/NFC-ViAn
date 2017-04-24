#ifndef BOOKMARK_H
#define BOOKMARK_H
#include <QJsonObject>
#include <QString>
#include <string>
/**
 * @brief The Bookmark class
 * Bookmark class is used for storing bookmarks, i.e. user
 * marked points in a video and an associated frame.
 */
class Bookmark {
public:
    Bookmark(int frame_nbr, QString file_pth, QString string);
    Bookmark();
    int get_frame_number();
    QString get_file_path();
    QString get_description();
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
private:
    int frame_number;       // Frame at which the bookmark was taken
    QString file_path;      // File path to the frame image associated with the bookmark
    QString description;    // Description for the bookmark, given by user

};

#endif // BOOKMARK_H
