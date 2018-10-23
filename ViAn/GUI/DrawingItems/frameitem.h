#ifndef FRAMEITEM_H
#define FRAMEITEM_H

#include "shapeitem.h"

class FrameItem : public ShapeItem {
    int m_frame;
public:
    FrameItem(int frame);

    int get_frame();
    void remove() override;
    Shapes* get_shape() override;
    ~FrameItem() override;
};

#endif // FRAMEITEM_H
