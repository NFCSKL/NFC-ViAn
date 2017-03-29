#ifndef ARROW_H
#define ARROW_H

#include "Shape.h"

class Arrow : public Shape {
public:
    Arrow(QColor col, QPoint pos);
    void draw(QImage &img) override;
    void handle_new_pos(QPoint pos);
};

#endif // ARROW_H
