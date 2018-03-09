#include "frameitem.h"

FrameItem::FrameItem(int frame) : TreeItem(FRAME_ITEM) {
    m_frame = frame;
    setText(0, QString::number(frame));
}

void FrameItem::remove(){}

void FrameItem::rename(){}

int FrameItem::get_frame() {
    return m_frame;
}
