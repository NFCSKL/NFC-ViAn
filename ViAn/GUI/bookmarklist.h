#ifndef BOOKMARKLIST_H
#define BOOKMARKLIST_H

#include <QListWidget>
#include <QBoxLayout>
#include <QMouseEvent>
#include <QDropEvent>
#include <QPoint>

#include "Project/bookmark.h"

/**
 * @brief The BookmarkList class
 */
class BookmarkList : public QListWidget{
    Q_OBJECT
    QPoint drag_start_pos;
    QListWidgetItem* clicked_item = nullptr;

    std::string m_par_cont_name = "";
    int m_container_type = UNSORTED;
    bool m_accept_container = true;
public:
    BookmarkList(bool accept_container = true, int container_type = UNSORTED, QWidget* parent = nullptr);

    QListWidgetItem* get_clicked_item();

    void set_parent_name(std::string name);
public slots:
    void on_parent_name_edited(QString name);
private:
    void item_left_clicked();
    void item_right_clicked(const QPoint pos);
private slots:
    void rename_item();
    void remove_item();
    void on_double_clicked(QListWidgetItem* item);
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
signals:
    set_bookmark_video(VideoProject* vid_proj, int frame_idx);
};

#endif // BOOKMARKLIST_H
