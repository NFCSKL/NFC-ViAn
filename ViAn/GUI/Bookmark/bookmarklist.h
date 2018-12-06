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
    int m_list_type = UNSORTED;
    bool m_accept_container = true;
public:
    BookmarkList(bool accept_container = true, int container_type = UNSORTED, QWidget* parent = nullptr);
    ~BookmarkList() override;

    QListWidgetItem* get_clicked_item();

    int category_cnt = 1;
    QString get_parent_name();
    void set_parent_name(QString &name);
    void update_index();
public slots:
    void on_parent_name_edited(QString name);
private:
    void item_right_clicked(const QPoint pos);
    bool bookmark_drop(QDropEvent *event);
    bool container_drop(QDropEvent *event);
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
    void add_category(QString name);
};

#endif // BOOKMARKLIST_H
