#ifndef ARROWITEM_H
#define ARROWITEM_H

#include "shapeitem.h"
#include "Video/shapes/arrow.h"

class Arrow;
class ArrowItem : public ShapeItem {
    Arrow* m_arrow;
public:
    ArrowItem(Arrow* arrow);
    ~ArrowItem() override;
    Arrow* get_shape() override;
    void remove() override;
};

#endif // ARROWITEM_H
