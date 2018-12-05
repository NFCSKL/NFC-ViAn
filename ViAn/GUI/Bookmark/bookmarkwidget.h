#ifndef BOOKMARKWIDGET_H
#define BOOKMARKWIDGET_H

#include "Project/video.h"

#include <opencv2/core/core.hpp>

#include <QWidget>

class BookmarkCategory;
class BookmarkItem;
class BookmarkList;
class Project;
class QDockWidget;
class QScrollArea;
class QVBoxLayout;
class VideoProject;

enum list_types {BOOKMARK, CONTAINER};
class BookmarkWidget : public QWidget
{
    Q_OBJECT
    Project* m_proj = nullptr;
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
    BookmarkCategory *add_new_folder(QString name);
    void create_bookmark(VideoProject *vid_proj, VideoState state, cv::Mat bookmark_frame, cv::Mat org_frame, QString time, QString description);
    void export_original_frame(VideoProject *vid_proj, const int frame_nbr, cv::Mat frame);
    void save_item_data();
    void set_project(Project* proj);
    void clear_bookmarks();
    void generate_report();

private:
    void add_to_container(BookmarkItem* bm_item, int type);
    QString get_input_text(QString text, bool* ok);
    void load_bookmarks();
};

#endif // BOOKMARKWIDGET_H
