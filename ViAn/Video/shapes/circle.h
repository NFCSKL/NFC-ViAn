#ifndef CIRCLE_H
#define CIRCLE_H

#include "shapes.h"

class Circle : public Shapes {
public:
    Circle();
    Circle(QColor col, QPoint pos);
    ~Circle() override;
    cv::Mat draw(cv::Mat &frame) override;
    cv::Mat draw_scaled(cv::Mat &frame, cv::Point anchor, double scale_factor, int angle, int width, int height) override;
    void handle_new_pos(QPoint pos) override;
    void write(QJsonObject& json) override;
    void read(const QJsonObject& json) override;
    QString get_name() override;
    void set_name(QString name) override;
private:
    QString m_name = "Circle";
};

#endif // CIRCLE_H
