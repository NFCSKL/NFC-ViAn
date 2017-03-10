#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

class rectangle : public shape {
public:
    rectangle(QColor col, QPoint pos);
    void draw(QImage &img) override;
};

#endif // RECTANGLE_H
