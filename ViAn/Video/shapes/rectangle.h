#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shapes.h"

class Rectangle : public Shapes {
public:
    Rectangle();
    Rectangle(QColor col, QPoint pos);
    ~Rectangle() override;
    cv::Mat draw_scaled(cv::Mat &frame, cv::Point anchor,
                        double scale_factor, int angle,
                        bool flip_h, bool flip_v,
                        int width, int height) override;
    void handle_new_pos(QPoint pos) override;
    void write(QJsonObject& json) override;
    void read(const QJsonObject& json) override;
private:

};

#endif // RECTANGLE_H
