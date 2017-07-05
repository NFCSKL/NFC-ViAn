#ifndef BOOKMARKWIDGET_H
#define BOOKMARKWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QDebug>
#include <QScrollArea>
#include <QDockWidget>
#include <opencv2/opencv.hpp>
#include "Project/videoproject.h"
#include "bookmarkitem.h"
#include "bookmarkcategory.h"
#include "bookmarklist.h"

enum list_types {BOOKMARK, CONTAINER};

class BookmarkWidget : public QWidget
{
    Q_OBJECT
    std::string m_path;
    BookmarkList* bm_list;
    QVBoxLayout* bm_list_layout;
    QVBoxLayout* layout;
    QScrollArea* scroll_area;
    QDockWidget* folder_dock;
    const int TEXT_EDIT_MIN_HEIGHT = 64;
public:
    explicit BookmarkWidget(QWidget *parent = nullptr);

signals:
    void play_bookmark_video(VideoProject* vid_proj, int frame_idx);
public slots:
    void create_bookmark(VideoProject *vid_proj, const int frame_nbr, cv::Mat);
    void load_bookmarks(VideoProject *vid_proj);
    void set_path(std::string path);
private slots:
//    void item_context_menu(QPoint pos);
private:
    void add_new_folder();
    QString get_input_text(std::string text, bool* ok);
};

#endif // BOOKMARKWIDGET_H
