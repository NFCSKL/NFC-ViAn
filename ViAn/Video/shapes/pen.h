#ifndef PEN_H
#define PEN_H

#include "shape.h"

class pen : public shape {
public:
    pen(QColor col, QPoint pos);
    void draw(QImage &img) override;
    void add_drawing_pos(QPoint pos);
private:
    QVector<QLine> lines;
};

#endif // PEN_H
