#ifndef ARROW_H
#define ARROW_H

#include "shapes.h"

class Arrow : public Shapes {
public:
    Arrow();
    Arrow(QColor col, QPoint pos);
    ~Arrow() override;
    cv::Mat draw(cv::Mat &frame) override;
    cv::Mat draw_scaled(cv::Mat &frame, cv::Point anchor, double scale_factor) override;
    void handle_new_pos(QPoint pos) override;
    void write(QJsonObject& json) override;
    void read(const QJsonObject& json) override;
    QString get_name() override;
    void set_name(QString name) override;

private:
    QString m_name = "Arrow";
};

#endif // ARROW_H
