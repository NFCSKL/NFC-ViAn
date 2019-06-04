#ifndef YOLOWIDGETITEM_H
#define YOLOWIDGETITEM_H

#include <QListWidgetItem>

#include "opencv2/highgui/highgui.hpp"

class YoloWidgetItem : public QListWidgetItem
{
public:
    YoloWidgetItem(QListWidget* parent = nullptr, int type = Type);
    int frame_nr;
    QString class_name;
    double confidence;
    cv::Rect rect;
    ~YoloWidgetItem();
};

#endif // YOLOWIDGETITEM_H
