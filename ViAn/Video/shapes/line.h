#ifndef LINE_H
#define LINE_H

#include "shapes.h"

class Line : public Shapes {
public:
    Line();
    Line(QColor col, QPoint pos);
    ~Line() override;
    cv::Mat draw_scaled(cv::Mat &frame, cv::Point anchor,
                        double scale_factor, int angle,
                        bool flip_h, bool flip_v,
                        int width, int height) override;
    void handle_new_pos(QPoint pos) override;
    void write(QJsonObject& json) override;
    void read(const QJsonObject& json) override;

private:

};

#endif // LINE_H
