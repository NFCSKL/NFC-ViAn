#include "videoedititem.h"

VideoEditItem::VideoEditItem(QString path, int start, int end, int last_frame, QListWidget* parent) : QListWidgetItem(parent) {
    m_path = path;
    m_start = start;
    m_end = end;
    m_last_frame = last_frame;
    set_name();
}

void VideoEditItem::set_name() {
    QString name = QString::number(m_start) + " - " + QString::number(m_end);
    setText(name);
}

int VideoEditItem::get_start() {
    return m_start;
}

int VideoEditItem::get_end() {
    return m_end;
}

int VideoEditItem::get_last_frame() {
    return m_last_frame;
}

void VideoEditItem::set_start(int new_start) {
    m_start = new_start;
    set_name();
}

void VideoEditItem::set_end(int new_end) {
    m_end = new_end;
    set_name();
}
