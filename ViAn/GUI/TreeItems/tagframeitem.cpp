#include "tagframeitem.h"

TagFrameItem::TagFrameItem(int frame) : TreeItem(TAG_FRAME_ITEM) {
    m_frame = frame;
    setText(0, QString::number(m_frame));
}

void TagFrameItem::remove(){}

void TagFrameItem::rename() {
    // TODO
}

int TagFrameItem::get_frame() {
    return m_frame;
}
