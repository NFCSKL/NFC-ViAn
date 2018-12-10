#ifndef BOOKMARKFOLDER_H
#define BOOKMARKFOLDER_H

#include "Project/video.h"

#include <QListWidget>
#include <QObject>

class BookmarkItem;
class BookmarkList;
class Project;
class QScrollArea;
class QVBoxLayout;
class VideoProject;

/**
 * @brief The BookmarkCategory class
 * This class is inteded to store two scrollable lists
 * containing bookmarks.
 */
class BookmarkCategory : public QObject, public QListWidgetItem {
    Q_OBJECT
    // Title line
    QLineEdit* m_title = nullptr;
    // Category name
    QString m_name = "";
    // Index in list
    int m_index;
    // ID to identify the categories
    int id = -1;
    // Project holding category
    Project* m_proj = nullptr;
    // Widget layout
    QVBoxLayout* layout = nullptr;
    // Scroll areas for disp & ref
    QScrollArea* disputed = nullptr;
    QScrollArea* reference = nullptr;
    // Disp & ref lists
    BookmarkList* disp_list = nullptr;
    BookmarkList* ref_list = nullptr;

    QWidget* folder = new QWidget();


public:
    explicit BookmarkCategory(QString name, int type = 1);
    ~BookmarkCategory();
    bool m_unsaved_changes = true;      // Track whether the class instance has unsaved changes

    // Add item to disp or ref list
    void add_disp_item(BookmarkItem* bm);
    void add_ref_item(BookmarkItem* bm);

    void update_index(int index);
    void set_index(int index);
    void set_project(Project* proj);
    void set_id(const int& new_id);

    QString get_name();
    Project* get_project();
    int get_index();
    int get_id() const;
    std::vector<BookmarkItem*> get_disputed();
    std::vector<BookmarkItem*> get_references();
    QWidget* get_folder();

    void update_title(const QString &title);
    BookmarkCategory* copy();
    bool is_saved() const;

private:  
    QScrollArea* make_scrollable_container(BookmarkList* cont);

private slots:
    void on_text_edited(QString name);

signals:
    void set_bookmark_video(VideoProject* vid_proj, VideoState state);
};

#endif // BOOKMARKFOLDER_H
