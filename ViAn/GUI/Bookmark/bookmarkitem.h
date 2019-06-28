#ifndef BOOKMARKITEM_H
#define BOOKMARKITEM_H

#include "bookmarkbaseitem.h"
#include <QListWidgetItem>

class Bookmark;
class BookmarkItem : public BookmarkBaseItem {
    QString hover_text;
public:
    BookmarkItem(Bookmark *bookmark, int type = 1);
    ~BookmarkItem() override;
    void update_item(const int &index, const int &cat_id, const int &type);
    Bookmark* get_bookmark();
    int get_frame_number();
    QString get_file_path();
    void update_description(const QString &text);
    void set_thumbnail(QString thum_path);
    BookmarkItem* copy();
    QString get_description() const;
    QString get_time() const;

private:
    Bookmark* m_bookmark = nullptr;
};

#endif // BOOKMARKITEM_H
