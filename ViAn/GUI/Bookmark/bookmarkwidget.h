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
#include "reportgenerator.h"
#include <QThread>
class BookmarkCategory;
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
    QThread* processing_thread;
    const int TEXT_EDIT_MIN_HEIGHT = 64;
    int category_cnt = 1;
public:
    explicit BookmarkWidget(QWidget *parent = nullptr);
signals:
    void play_bookmark_video(VideoProject* vid_proj, int frame_idx);
public slots:
    void create_bookmark(VideoProject *vid_proj, const int frame_nbr, cv::Mat bookmark_frame, cv::Mat org_frame, QString time);
    void export_original_frame(VideoProject *vid_proj, const int frame_nbr, cv::Mat frame);
    void load_bookmarks(VideoProject *vid_proj);
    void set_path(std::string path);
    void clear_bookmarks();
private slots:
//    void item_context_menu(QPoint pos);
private:
    void add_new_folder();
    void generate_report();
    BookmarkCategory* add_to_container(BookmarkItem* bm_item, std::pair<int, std::string> *container);
    QString get_input_text(QString text, bool* ok);
};

#endif // BOOKMARKWIDGET_H
