#ifndef TEXT_H
#define TEXT_H

#include "shapes.h"

class Text : public Shapes {
public:
    Text();
    Text(QColor col, QPoint pos, QString strng, double fnt_scl);
    ~Text() override;
    cv::Mat draw_scaled(cv::Mat &frame, cv::Point anchor, double scale_factor, int angle, int width, int height) override;
    void handle_new_pos(QPoint pos) override;
    void write(QJsonObject& json) override;
    void read(const QJsonObject& json) override;
    double set_font_scale(QPoint);
    double get_font_scale();
    cv::Size get_text_size();
    void set_text_size(cv::Size size);

    void update_text_pos(QPoint pos);
    void update_text_draw_end();
    cv::Point get_text_draw_end();

private:
    double font_scale;
    cv::Size text_size = cv::Size(-1, -1);
    int baseline = 0;
};

#endif // TEXT_H
