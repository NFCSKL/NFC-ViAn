#ifndef TEXT_H
#define TEXT_H

#include "shape.h"

class text : public shape {
public:
    text(QColor col, QPoint pos, QString strng);
    cv::Mat draw(cv::Mat &frame) override;
    void handle_new_pos(QPoint pos);
private:
    const int FONT_SIZE = 24;
    QString string;
};

#endif // TEXT_H
