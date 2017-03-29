#ifndef LINE_H
#define LINE_H

#include "Shape.h"

class Line : public Shape {
public:
    Line(QColor col, QPoint pos);
    void draw(QImage &img) override;
    void handle_new_pos(QPoint pos);
};

#endif // LINE_H
