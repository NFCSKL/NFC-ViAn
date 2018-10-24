#ifndef BOOKMARKWIDGET_H
#define BOOKMARKWIDGET_H

#include "Project/video.h"

#include <opencv2/core/core.hpp>

#include <QWidget>

class BookmarkCategory;
class BookmarkItem;
class BookmarkList;
class QDockWidget;
class QScrollArea;
class QVBoxLayout;
class VideoProject;

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

public:
    explicit BookmarkWidget(QWidget *parent = nullptr);
signals:
    void play_bookmark_video(VideoProject* vid_proj, VideoState state);
    void set_status_bar(QString);
    void play_video(bool);
    void show_bm_dock(bool);
public slots:
    void create_bookmark(VideoProject *vid_proj, VideoState state, cv::Mat bookmark_frame, cv::Mat org_frame, QString time, QString description);
    void export_original_frame(VideoProject *vid_proj, const int frame_nbr, cv::Mat frame);
    void load_bookmarks(VideoProject *vid_proj);
    void set_path(std::string path);
    void clear_bookmarks();
    void generate_report();
private slots:
//    void item_context_menu(QPoint pos);
private:
    BookmarkCategory* add_to_container(BookmarkItem* bm_item, std::pair<int, std::string> *container);
    QString get_input_text(QString text, bool* ok);
};

#endif // BOOKMARKWIDGET_H
