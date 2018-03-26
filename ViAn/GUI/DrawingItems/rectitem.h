#ifndef RECTITEM_H
#define RECTITEM_H

#include "shapeitem.h"
#include "Video/shapes/rectangle.h"

class RectItem : public ShapeItem {
    Rectangle* m_rect;
public:
    RectItem(Rectangle* rect);
    Rectangle* get_shape();
    void rename();
    void remove();
    ~RectItem() override;
};

#endif // RECTITEM_H
