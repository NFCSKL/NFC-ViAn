#ifndef BOOKMARKVIEW_H
#define BOOKMARKVIEW_H

#include <QListWidget>

class BookmarkView {
public:
    BookmarkView(QListWidget* parent);
    void add_bookmark(std::string file_path);
private:
    const int BOOKMARK_HEIGHT = 128;
    QListWidget* view;
};

#endif // BOOKMARKVIEW_H
