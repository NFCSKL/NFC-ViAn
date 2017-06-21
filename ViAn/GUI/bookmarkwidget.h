#ifndef BOOKMARKWIDGET_H
#define BOOKMARKWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include "bookmarkitem.h"
class BookmarkWidget : public QWidget
{
    const int TEXT_EDIT_MIN_HEIGHT = 64;
    Q_OBJECT
public:
    explicit BookmarkWidget(QWidget *parent = nullptr);
    void add_bookmark(std::string description, std::vector<std::string> tags);

signals:

public slots:
    void create_bookmark(const int frame_nbr, cv::Mat);
private:
    QListWidget* bookmark_list;
    QString get_input_text(std::string bookmark_text, bool* ok);
};

#endif // BOOKMARKWIDGET_H
