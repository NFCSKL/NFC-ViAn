#ifndef LINE_H
#define LINE_H

#include "shapes.h"

class Line : public Shapes {
public:
    Line();
    Line(QColor col, QPoint pos);
    ~Line() override;
    cv::Mat draw(cv::Mat &frame) override;
    cv::Mat draw_scaled(cv::Mat &frame, cv::Point anchor, double scale_factor) override;
    void handle_new_pos(QPoint pos) override;
    void write(QJsonObject& json) override;
    void read(const QJsonObject& json) override;
    QString get_name() override;
    void set_name(QString name) override;
private:
    QString m_name = "Line";
};

#endif // LINE_H
