#ifndef TEXT_H
#define TEXT_H

#include "shapes.h"

class Text : public Shapes {
public:
    Text();
    Text(QColor col, QPoint pos, QString strng, double fnt_scl);
    ~Text() override;
    cv::Mat draw(cv::Mat &frame) override;
    cv::Mat draw_scaled(cv::Mat &frame, cv::Point anchor, double scale_factor, int angle, int width, int height) override;
    void handle_new_pos(QPoint pos) override;
    void write(QJsonObject& json) override;
    void read(const QJsonObject& json) override;
    QString get_name() override;
    void set_name(QString name) override;
    double set_font_scale(QPoint);
    double get_font_scale();

private:
    QString m_name = "Enter text";
    double font_scale;
    cv::Size text_size;
    int baseline = 0;
};

#endif // TEXT_H
