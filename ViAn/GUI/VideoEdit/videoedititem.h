#ifndef VIDEOEDITITEM_H
#define VIDEOEDITITEM_H

#include <QListWidgetItem>

class VideoEditItem : public QListWidgetItem {
    QString m_path = "";
    int m_start = -1;
    int m_end = -1;
    int m_last_frame = -1;

    void set_name();
public:
    VideoEditItem(QString path, int start, int end, int last_frame, QListWidget *parent = nullptr);

    int get_start();
    int get_end();
    int get_last_frame();
    void set_start(int new_start);
    void set_end(int new_end);
};

#endif // VIDEOEDITITEM_H
