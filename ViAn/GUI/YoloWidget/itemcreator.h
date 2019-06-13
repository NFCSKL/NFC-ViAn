#ifndef ITEMCREATOR_H
#define ITEMCREATOR_H

#include <QObject>

class Analysis;
class YoloWidgetItem;
class ItemCreator : public QObject {
    Q_OBJECT
    Analysis* m_analysis = nullptr;
    std::vector<YoloWidgetItem*>* m_detection_list;
    std::vector<int> m_frame_list;
    QString video_path;
    QString proj_dir;

    int last_frame = 100;

public:
    ItemCreator(Analysis* analysis, std::vector<YoloWidgetItem*>* detection_list, std::vector<int> frame_list, QString v_path, QString p_path);

public slots:
    void create_detection_items();

signals:
    void detection_added();
    void send_last_frame(int);
    void finished();
};

#endif // ITEMCREATOR_H
