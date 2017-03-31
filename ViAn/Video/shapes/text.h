#ifndef TEXT_H
#define TEXT_H

#include "shape.h"

class Text : public Shape {
public:
    Text(QColor col, QPoint pos, QString strng, double fnt_scl);
    cv::Mat draw(cv::Mat &frame) override;
    void handle_new_pos(QPoint pos) override;

    // Constants describing the limit and presicion of the font scale value.
    static constexpr double FONT_SCALE_MIN = 0.5, FONT_SCALE_MAX = 5.0, FONT_SCALE_STEP = 0.1;
    static constexpr int FONT_SCALE_DECIMALS = 1;
private:
    QString string;
    double font_scale;
};

#endif // TEXT_H
