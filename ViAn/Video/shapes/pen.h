#ifndef PEN_H
#define PEN_H

#include "shapes.h"

class Pen : public Shapes {
public:
    Pen();
    Pen(QColor col, QPoint pos);
    ~Pen() override;
    cv::Mat draw_scaled(cv::Mat &frame, cv::Point anchor,
                        double scale_factor, int angle,
                        bool flip_h, bool flip_v,
                        int width, int height) override;
    void handle_new_pos(QPoint pos) override;
    void move_shape(QPoint p) override;
    void write(QJsonObject& json) override;
    void read(const QJsonObject& json) override;
    std::vector<cv::Point> get_points();
    void set_points(std::vector<cv::Point> new_Points);
private:
    std::vector<cv::Point> points;
};

#endif // PEN_H
