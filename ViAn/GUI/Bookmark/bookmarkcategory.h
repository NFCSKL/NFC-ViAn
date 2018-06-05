#ifndef BOOKMARKFOLDER_H
#define BOOKMARKFOLDER_H
#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QDebug>
#include <QScrollArea>
#include <QCommonStyle>
#include <QMenu>
#include "bookmarkitem.h"
#include "bookmarklist.h"
/**
 * @brief The BookmarkCategory class
 * This class is inteded to store two scrollable lists
 * containing bookmarks or images.
 */
class BookmarkCategory : public QObject, public QListWidgetItem
{
    Q_OBJECT
    // Title line
    QLineEdit* m_title = nullptr;
    // Category name
    std::string m_name;
    // Widget layout
    QVBoxLayout*  layout = nullptr;
    // Scroll areas for disp_ref
    QScrollArea* disputed = nullptr;
    QScrollArea* reference = nullptr;
    // Disp ref lists
    BookmarkList* disp_list = nullptr;
    BookmarkList* ref_list = nullptr;
public:
    explicit BookmarkCategory(std::string name, QListWidget *parent = nullptr, int type = 0);
    ~BookmarkCategory();
    // Add item to disp list
    void add_disp_item(BookmarkItem* bm);
    // Add item to ref list
    void add_ref_item(BookmarkItem* bm);

    std::string get_name();
    std::vector<BookmarkItem*> get_disputed();
    std::vector<BookmarkItem*> get_references();

    void update_title(const QString &title);
    BookmarkCategory* copy(QListWidget* new_parent);
private:  
    QScrollArea* make_scrollable_container(BookmarkList* cont);
private slots:
    void on_text_edited(QString name);
signals:
    void set_bookmark_video(VideoProject* vid_proj, VideoState state);
};

#endif // BOOKMARKFOLDER_H
