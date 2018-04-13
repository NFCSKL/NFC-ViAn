#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shapes.h"

class Rectangle : public Shapes {
public:
    Rectangle();
    Rectangle(QColor col, QPoint pos);
    ~Rectangle() override;
    cv::Mat draw(cv::Mat &frame) override;
    void handle_new_pos(QPoint pos) override;
    void write(QJsonObject& json) override;
    void read(const QJsonObject& json) override;
    QString get_name() override;
    void set_name(QString) override;
private:
    QString m_name = "Rectangle";
};

#endif // RECTANGLE_H
