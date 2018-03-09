#ifndef CIRCLE_H
#define CIRCLE_H

#include "shapes.h"

class Circle : public Shapes {
public:
    Circle();
    Circle(QColor col, QPoint pos);
    cv::Mat draw(cv::Mat &frame) override;
    void handle_new_pos(QPoint pos) override;
    void write(QJsonObject& json) override;
    void read(const QJsonObject& json) override;
};

#endif // CIRCLE_H
