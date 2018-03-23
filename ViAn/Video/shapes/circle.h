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
    QString get_name() override;
    void set_name(QString name) override;
private:
    QString m_name = "Circle";
};

#endif // CIRCLE_H
