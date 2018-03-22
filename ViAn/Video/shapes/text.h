#ifndef TEXT_H
#define TEXT_H

#include "shapes.h"

class Text : public Shapes {
public:
    Text();
    Text(QColor col, QPoint pos, QString strng, double fnt_scl);
    cv::Mat draw(cv::Mat &frame) override;
    void handle_new_pos(QPoint pos) override;
    void write(QJsonObject& json) override;
    void read(const QJsonObject& json) override;
    void set_text(QString text);
    QString get_text();
    void set_font_scale(QPoint);

    // Constants describing the limit and precision of the font scale value.
    static constexpr double FONT_SCALE_MIN = 0.5, FONT_SCALE_MAX = 5.0, FONT_SCALE_STEP = 0.1;
    static constexpr int FONT_SCALE_DECIMALS = 1;
private:
    QString string = "";
    double font_scale;
    cv::Size text_size;
    int baseline = 0;
};

#endif // TEXT_H
