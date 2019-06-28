#ifndef CIRCLEITEM_H
#define CIRCLEITEM_H

#include "shapeitem.h"
#include "Video/shapes/circle.h"

class CircleItem : public ShapeItem {
    Circle* m_circle;
public:
    CircleItem(Circle *circle);
    Circle* get_shape() override;
    void remove() override;
    ~CircleItem() override;
};

#endif // CIRCLEITEM_H
