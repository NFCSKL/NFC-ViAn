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
    QString m_path;
    BookmarkList* bm_list;
    QVBoxLayout* bm_list_layout;
    QVBoxLayout* layout;
    QScrollArea* scroll_area;
    QDockWidget* folder_dock;
    QThread* processing_thread;

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
    void save_item_data();
    void set_path(QString path);
    void clear_bookmarks();
    void generate_report();

private:
    BookmarkCategory* add_to_container(BookmarkItem* bm_item, std::pair<int, QString> *container);
    QString get_input_text(QString text, bool* ok);
};

#endif // BOOKMARKWIDGET_H
