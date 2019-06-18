#include "yolowidgetitem.h"

#include <QDebug>
#include <QFile>

YoloWidgetItem::YoloWidgetItem(QListWidget *parent, int type) : QListWidgetItem(parent, type) {

}

YoloWidgetItem::~YoloWidgetItem() {
    QFile file(thumbnail_path);
    if (file.exists()) file.remove();
}

void YoloWidgetItem::set_icon(QString icon_path) {
    const QIcon icon(icon_path);
    setIcon(icon);
    thumbnail_path = icon_path;
}
