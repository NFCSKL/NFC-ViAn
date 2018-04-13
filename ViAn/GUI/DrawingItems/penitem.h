#ifndef PENITEM_H
#define PENITEM_H

#include "shapeitem.h"
#include "Video/shapes/pen.h"

class PenItem : public ShapeItem {
    Pen* m_pen;
public:
    PenItem(Pen* pen);
    Pen* get_shape();
    void update_shape_name();
    void remove();
    ~PenItem() override;
};

#endif // PENITEM_H
