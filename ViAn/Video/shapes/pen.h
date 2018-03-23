#ifndef PEN_H
#define PEN_H

#include "shapes.h"

class Pen : public Shapes {
public:
    Pen();
    Pen(QColor col, QPoint pos);
    cv::Mat draw(cv::Mat &frame) override;
    void handle_new_pos(QPoint pos) override;
    void write(QJsonObject& json) override;
    void read(const QJsonObject& json) override;
    QString get_name() override;
    void set_name(QString name) override;
private:
    std::vector<std::pair<cv::Point, cv::Point>> lines;
    QString m_name = "Pen";
};

#endif // PEN_H
