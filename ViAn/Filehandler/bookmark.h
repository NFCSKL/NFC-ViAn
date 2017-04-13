#ifndef BOOKMARK_H
#define BOOKMARK_H
#include <QJsonObject>
#include <QString>
#include <string>

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
    int frame_number;
    QString file_path;
    QString description;

};

#endif // BOOKMARK_H
