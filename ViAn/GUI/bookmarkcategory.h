#ifndef BOOKMARKFOLDER_H
#define BOOKMARKFOLDER_H
#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QDebug>
#include <QScrollArea>
#include <QCommonStyle>
#include <QMenu>
#include "bookmarkwidget.h"
#include "bookmarklist.h"
/**
 * @brief The BookmarkCategory class
 * This class is inteded to store two scrollable lists
 * containing bookmarks or images.
 */
class BookmarkCategory : public QObject, public QListWidgetItem
{
    Q_OBJECT
    QLineEdit* m_title = nullptr;
    QVBoxLayout*  layout = nullptr;
    QScrollArea* disputed = nullptr;
    QScrollArea* reference = nullptr;
    BookmarkList* disp_list = nullptr;
    BookmarkList* ref_list = nullptr;
    std::string m_name;
public:
    explicit BookmarkCategory(std::string name, QListWidget *parent = nullptr, int type = 0);
    ~BookmarkCategory();
    void add_disp_item(BookmarkItem* bm);
    void add_ref_item(BookmarkItem* bm);

    std::string get_name();
    std::vector<BookmarkItem*> get_disputed();
    std::vector<BookmarkItem*> get_references();
    QMenu* create_menu();
    void update_title(const QString &title);
    BookmarkCategory* copy(QListWidget* new_parent);
private:
    QScrollArea* make_scrollable_container(BookmarkList* cont);
private slots:
    void on_text_edited(QString name);
signals:
    set_bookmark_video(VideoProject* vid_proj, int frame_idx);
};

#endif // BOOKMARKFOLDER_H
