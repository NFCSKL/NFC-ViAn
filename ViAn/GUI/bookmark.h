#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QListWidgetItem>

class Bookmark : public QListWidgetItem {
public:
    Bookmark(int frame_nbr, QImage img, QString string, QListWidget* view);
    int get_frame_number();
private:
    int frame_number;
};

#endif // BOOKMARK_H
