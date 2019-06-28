#include "tagframeitem.h"

#include "Project/Analysis/tagframe.h"

TagFrameItem::TagFrameItem(TagFrame* t_frame) : TreeItem(TAG_FRAME_ITEM) {
    m_t_frame = t_frame;
    m_frame = t_frame->m_frame;
    m_name = t_frame->name;
    setText(0, m_name);
    setFlags(flags() & ~Qt::ItemIsEditable);
}

VideoState TagFrameItem::get_state() {
    VideoState state;
    state = m_t_frame->get_state();
    return state;
}

void TagFrameItem::remove(){}

void TagFrameItem::rename() {
    // TODO
}

int TagFrameItem::get_frame() {
    return m_t_frame->get_state().frame;
}
