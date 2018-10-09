#include "sequencetagitem.h"

SequenceTagItem::SequenceTagItem(const std::string& name, const int& index, TagFrame* tag) : TreeItem(SEQUENCE_TAG_ITEM) {
    m_index = index;
    setText(0, QString::fromStdString(name));
}

int SequenceTagItem::get_index() {
    return m_index;
}

void SequenceTagItem::remove(){}

void SequenceTagItem::rename(){}
