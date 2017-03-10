#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

class circle : public shape {
public:
    circle(QColor col, QPoint pos);
    void draw(QImage &img) override;
    void add_drawing_pos(QPoint pos);
};

#endif // CIRCLE_H
