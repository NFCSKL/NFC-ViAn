#ifndef LINE_H
#define LINE_H

#include "shape.h"

class line : public shape {
public:
    line(QColor col, QPoint pos);
    void draw(QImage &img) override;
};

#endif // LINE_H
