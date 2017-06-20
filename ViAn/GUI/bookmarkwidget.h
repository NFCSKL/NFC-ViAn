#ifndef BOOKMARKWIDGET_H
#define BOOKMARKWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include "Project/videoproject.h"
#include "bookmarkitem.h"
class BookmarkWidget : public QWidget
{
    const int TEXT_EDIT_MIN_HEIGHT = 64;
    std::map<std::string, BookmarkItem> m_vid_bm_map;
    Q_OBJECT
public:
    explicit BookmarkWidget(QWidget *parent = nullptr);
    void add_bookmark(BookmarkItem* bm_item);

signals:

public slots:
    void create_bookmark(VideoProject *vid_proj, const int frame_nbr, cv::Mat);
private:
    QListWidget* bookmark_list;
    void init_bookmark_list();
    QString get_input_text(std::string bookmark_text, bool* ok);
};

#endif // BOOKMARKWIDGET_H
