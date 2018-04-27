#ifndef PEN_H
#define PEN_H

#include "shapes.h"

class Pen : public Shapes {
public:
    Pen();
    Pen(QColor col, QPoint pos);
    ~Pen() override;
    cv::Mat draw(cv::Mat &frame) override;
    void handle_new_pos(QPoint pos) override;
    void move_shape(QPoint p) override;
    void write(QJsonObject& json) override;
    void read(const QJsonObject& json) override;
    QString get_name() override;
    void set_name(QString name) override;
    std::vector<cv::Point> get_points();
    void set_points(std::vector<cv::Point> new_Points);
private:
    std::vector<cv::Point> points;
    QString m_name = "Pen";
};

#endif // PEN_H
