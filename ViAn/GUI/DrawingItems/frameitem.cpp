#include "frameitem.h"

FrameItem::FrameItem(int frame) : ShapeItem(FRAME_ITEM) {
    m_frame = frame;
    setFlags(flags() | Qt::ItemIsDropEnabled);
    setText(0, QString::number(frame));
}

FrameItem::~FrameItem() {}

void FrameItem::remove(){}

void FrameItem::update_shape_name(){}

void FrameItem::update_shape_color(){}

Shapes* FrameItem::get_shape() {
    return nullptr;
}

int FrameItem::get_frame() {
    return m_frame;
}
