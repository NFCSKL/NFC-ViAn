#ifndef VIEWPATHITEM_H
#define VIEWPATHITEM_H

#include <QTreeWidgetItem>

class Video;
class ViewPathItem : public QTreeWidgetItem
{
    Video* m_video = nullptr;
    QString m_full_path, m_dir, m_name = "";
    bool is_changed = false;
    bool m_valid = false;
public:
    ViewPathItem(Video* video, QTreeWidget* parent = nullptr);

    void set_path(QString path);
    void set_dir(QString dir);
    void set_name(QString name);
    void set_valid(bool valid);

    QString get_path();
    QString get_dir();
    QString get_name();

    bool is_item_changed();
    bool is_valid();
    void update_video();
};

#endif // VIEWPATHITEM_H
