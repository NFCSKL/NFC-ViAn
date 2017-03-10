#ifndef PEN_H
#define PEN_H

#include "shape.h"

class pen : public shape {
public:
    pen(QColor col, QPoint pos);
    void update_drawing_pos(QPoint pos);
    void draw(QImage &img) override;
private:
    QList<QLine> lines;
};

#endif // PEN_H
