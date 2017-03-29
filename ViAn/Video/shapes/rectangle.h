#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

class rectangle : public shape {
public:
    rectangle(QColor col, QPoint pos);
    void draw(QImage &img) override;
    void handle_new_pos(QPoint pos) override;
};

#endif // RECTANGLE_H
