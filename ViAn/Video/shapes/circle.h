#ifndef CIRCLE_H
#define CIRCLE_H

#include "shapes.h"

class Circle : public Shapes {
public:
    Circle();
    Circle(QColor col, QPoint pos);
    ~Circle() override;
    cv::Mat draw_scaled(cv::Mat &frame, cv::Point anchor,
                        double scale_factor, int angle,
                        bool flip_h, bool flip_v,
                        int width, int height) override;
    void handle_new_pos(QPoint pos) override;
    void write(QJsonObject& json) override;
    void read(const QJsonObject& json) override;
private:

};

#endif // CIRCLE_H
