#include "tagframeitem.h"

TagFrameItem::TagFrameItem(int frame) : TreeItem(TAG_FRAME_ITEM) {
    m_frame = frame;
    setText(0, QString::number(m_frame));
}

void TagFrameItem::set_state(TagFrame* t_frame) {
    m_t_frame = t_frame;
}

VideoState TagFrameItem::get_state() {
    return m_t_frame->m_state;
}

void TagFrameItem::remove(){}

void TagFrameItem::rename() {
    // TODO
}

int TagFrameItem::get_frame() {
    return m_frame;
}
