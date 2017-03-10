#ifndef ARROW_H
#define ARROW_H

#include "shape.h"

class arrow : public shape {
public:
    arrow(QColor col, QPoint pos);
    void draw(QImage &img) override;
    void add_drawing_pos(QPoint pos);
};

#endif // ARROW_H
