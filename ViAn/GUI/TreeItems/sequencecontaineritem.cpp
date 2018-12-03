#include "sequencecontaineritem.h"

void SequenceContainerItem::update_text() {
    this->setText(0, BASE_NAME + " [" + QString::number(childCount()) + "]");
}

SequenceContainerItem::SequenceContainerItem() : TreeItem(SEQUENCE_CONTAINER_ITEM) {}

void SequenceContainerItem::remove() {}

void SequenceContainerItem::rename() {}
