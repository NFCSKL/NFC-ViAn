#ifndef BOOKMARKITEM_H
#define BOOKMARKITEM_H
#include "Project/bookmark.h"
#include <QListWidgetItem>
#include <QMenu>
#include <QObject>

class BookmarkItem : public QListWidgetItem {
    QString hover_text;
public:
    BookmarkItem(Bookmark *bookmark, int type = 1);
    ~BookmarkItem();
    Bookmark* get_bookmark();
    int get_frame_number();
    QString get_file_path();
    void update_description(const QString &text);
    void set_thumbnail(std::string thum_path);
    BookmarkItem* copy();
    QString get_description() const;

   static const int BOOKMARK_THUMBNAIL_HEIGHT = 64;
private:

    Bookmark* m_bookmark = nullptr;
};

#endif // BOOKMARKITEM_H
