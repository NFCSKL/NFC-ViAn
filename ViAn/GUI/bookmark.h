#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QString>
#include <string>

class Bookmark {
public:
    Bookmark(int frame_nbr, std::string file_pth, QString string);
    int get_frame_number();
    std::string get_file_path();
    QString get_description();
private:
    int frame_number;
    std::string file_path;
    QString description;
};

#endif // BOOKMARK_H
