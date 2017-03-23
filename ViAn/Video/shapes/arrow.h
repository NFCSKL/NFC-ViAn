#ifndef ARROW_H
#define ARROW_H

#include "shape.h"

class arrow : public shape {
public:
    arrow(QColor col, QPoint pos);
    void draw(QImage &img) override;
};

#endif // ARROW_H
