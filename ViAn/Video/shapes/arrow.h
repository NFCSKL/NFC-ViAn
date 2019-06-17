#ifndef ARROW_H
#define ARROW_H

#include "shapes.h"

class Arrow : public Shapes {
public:
    Arrow();
    Arrow(QColor col, QPoint pos);
    ~Arrow() override;
    cv::Mat draw_scaled(cv::Mat &frame, cv::Point anchor,
                        double scale_factor, int angle,
                        bool flip_h, bool flip_v,
                        int width, int height) override;
    void handle_new_pos(QPoint pos) override;
    void write(QJsonObject& json) override;
    void read(const QJsonObject& json) override;

private:

};

#endif // ARROW_H
