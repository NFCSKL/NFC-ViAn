#ifndef FRAMEITEM_H
#define FRAMEITEM_H

#include "shapeitem.h"

class FrameItem : public ShapeItem {
    int m_frame;
public:
    FrameItem(int frame);

    int get_frame();
    void remove();
    void update_shape_name();
    void update_shape_color();
    Shapes* get_shape();
    ~FrameItem() override;
};

#endif // FRAMEITEM_H
