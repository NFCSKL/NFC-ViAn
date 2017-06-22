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
class BookmarkWidget : public QWidget
{
    Q_OBJECT
    QListWidget* bm_win;
    QVBoxLayout* bm_win_layout;
    QVBoxLayout* layout;
    QScrollArea* scroll_area;
    QDockWidget* folder_dock;
    const int TEXT_EDIT_MIN_HEIGHT = 64;
public:
    explicit BookmarkWidget(QWidget *parent = nullptr);
    static QListWidget *get_drag_drop_list(QWidget *parent);

signals:

public slots:
    void create_bookmark(VideoProject *vid_proj, const int frame_nbr, cv::Mat);
private:
    void add_new_folder();
    QString get_input_text(std::string text, bool* ok);
};

#endif // BOOKMARKWIDGET_H
