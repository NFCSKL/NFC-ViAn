#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

class circle : public shape {
public:
    circle(QColor col, QPoint pos);
    void draw(QImage &img) override;
    void handle_new_pos(QPoint pos) override;
};

#endif // CIRCLE_H
