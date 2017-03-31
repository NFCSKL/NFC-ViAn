#ifndef TEXT_H
#define TEXT_H

#include "shape.h"

class Text : public Shape {
public:
    Text(QColor col, QPoint pos, QString strng, double fnt_scl);
    cv::Mat draw(cv::Mat &frame) override;
    void handle_new_pos(QPoint pos) override;
private:
    QString string;
    double font_scale = 1;
};

#endif // TEXT_H
