#include "sequencetagitem.h"

#include "Project/Analysis/tagframe.h"

SequenceTagItem::SequenceTagItem(const QString &name, const int &index, TagFrame *tag) :
    TreeItem (SEQUENCE_TAG_ITEM) {
    m_index = index;
    m_tag = tag;
    setText(0, name);
}

int SequenceTagItem::get_index() {
    return m_index;
}

VideoState SequenceTagItem::get_state() {
    return m_tag->m_state;
}

void SequenceTagItem::remove() {}

void SequenceTagItem::rename() {}
