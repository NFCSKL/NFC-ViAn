#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"

class Circle : public Shape {
public:
    Circle(QColor col, QPoint pos);
    void draw(QImage &img) override;
    void handle_new_pos(QPoint pos);
};

#endif // CIRCLE_H
