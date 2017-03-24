#ifndef ZOOMRECTANGLE_H
#define ZOOMRECTANGLE_H

#include "shape.h"

class zoomrectangle : public shape {
public:
    zoomrectangle();
    zoomrectangle(QPoint pos);
    void set_start_pos(QPoint pos);
    void draw(QImage &img) override;
    void handle_new_pos(QPoint pos);
};

#endif // ZOOMRECTANGLE_H
