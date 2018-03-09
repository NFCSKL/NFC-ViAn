#ifndef ARROW_H
#define ARROW_H

#include "shapes.h"

class Arrow : public Shapes {
public:
    Arrow();
    Arrow(QColor col, QPoint pos);
    cv::Mat draw(cv::Mat &frame) override;
    void handle_new_pos(QPoint pos) override;
    void write(QJsonObject& json) override;
    void read(const QJsonObject& json) override;
};

#endif // ARROW_H
