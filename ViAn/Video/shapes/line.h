#ifndef LINE_H
#define LINE_H

#include "shape.h"

class line : public shape {
public:
    line(QColor col, QPoint pos);
    void draw(QImage &img) override;
    void add_drawing_pos(QPoint pos);
};

#endif // LINE_H
