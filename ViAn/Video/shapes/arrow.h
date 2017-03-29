#ifndef ARROW_H
#define ARROW_H

#include "shape.h"

class arrow : public shape {
public:
    arrow(QColor col, QPoint pos);
    void draw(QImage &img) override;
    void handle_new_pos(QPoint pos) override;
};

#endif // ARROW_H
