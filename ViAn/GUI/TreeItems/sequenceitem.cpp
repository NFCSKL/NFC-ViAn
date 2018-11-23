#include "sequenceitem.h"

SequenceItem::SequenceItem(const QString& name, const int& index) : TreeItem(SEQUENCE_ITEM) {
    m_index = index;
    setText(0, name);
//    setFlags(flags() | Qt::ItemIsDragEnabled);
}

int SequenceItem::get_index() {
    return m_index;
}

void SequenceItem::remove(){}

void SequenceItem::rename(){}
