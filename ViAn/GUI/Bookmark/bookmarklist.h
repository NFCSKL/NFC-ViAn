#ifndef BOOKMARKLIST_H
#define BOOKMARKLIST_H

#include "Project/bookmark.h"

#include <QListWidget>

class BookmarkCategory;

/**
 * @brief The BookmarkList class
 */
class BookmarkList : public QListWidget{
    Q_OBJECT
    QPoint drag_start_pos;
    QListWidgetItem* clicked_item = nullptr;

    QString m_par_cont_name = "";
    int m_container_type = UNSORTED;
    bool m_accept_container = true;
    int category_cnt = 1;
public:
    BookmarkList(bool accept_container = true, int container_type = UNSORTED, QWidget* parent = nullptr);
    ~BookmarkList() override;

    QListWidgetItem* get_clicked_item();

    QString get_parent_name();
    void set_parent_name(QString &name);
public slots:
    void on_parent_name_edited(QString name);
    BookmarkCategory *add_new_folder(QString name);
private:
    void item_right_clicked(const QPoint pos);
    void bookmark_drop(BookmarkList* source, QDropEvent *event);
    void container_drop(BookmarkList* source, QDropEvent *event);
    void bookmark_copy(BookmarkList* source, QDropEvent* event);
private slots:
    void rename_item();
    void remove_item();
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
signals:
    void set_bookmark_video(VideoProject* vid_proj, VideoState state);
};

#endif // BOOKMARKLIST_H
