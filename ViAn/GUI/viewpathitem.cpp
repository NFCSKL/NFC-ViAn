#include "viewpathitem.h"

#include "Project/video.h"

ViewPathItem::ViewPathItem(Video* video, QTreeWidget* parent) : QTreeWidgetItem (parent) {
    m_video = video;
    set_path(video->file_path);
}

void ViewPathItem::set_path(QString path) {
    int index = path.lastIndexOf('/');
    set_dir(path.left(index));
    set_name(path.mid(index+1));
}

void ViewPathItem::set_dir(QString dir) {
    m_dir = dir;
    m_full_path = m_dir + "/" + m_name;
    setText(2, m_full_path);
    is_changed = true;
}

void ViewPathItem::set_name(QString name) {
    m_name = name;
    m_full_path = m_dir + "/" + m_name;
    setText(1, m_name);
    setText(2, m_full_path);
    is_changed = true;
}

void ViewPathItem::set_valid(bool valid) {
    m_valid = valid;
}

QString ViewPathItem::get_path() {
    return m_full_path;
}

QString ViewPathItem::get_dir() {
    return m_dir;
}

QString ViewPathItem::get_name() {
    return m_name;
}

bool ViewPathItem::is_item_changed() {
    return is_changed;
}

bool ViewPathItem::is_valid() {
    return m_valid;
}

void ViewPathItem::update_video() {
    m_video->file_path = m_full_path;
    m_video->set_name(m_name);
}
