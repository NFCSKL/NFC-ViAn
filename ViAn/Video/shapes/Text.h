#ifndef TEXT_H
#define TEXT_H

#include "Shape.h"

class Text : public Shape {
public:
    Text(QColor col, QPoint pos, QString strng);
    cv::Mat draw(cv::Mat &frame) override;
    void handle_new_pos(QPoint pos) override;
private:
    const int FONT_SCALE = 1;
    QString string;
};

#endif // TEXT_H
