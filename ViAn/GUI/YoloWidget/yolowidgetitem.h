#ifndef YOLOWIDGETITEM_H
#define YOLOWIDGETITEM_H

#include <QListWidgetItem>

class YoloWidgetItem : public QListWidgetItem
{
public:
    YoloWidgetItem(QListWidget* parent = nullptr, int type = Type);
};

#endif // YOLOWIDGETITEM_H
