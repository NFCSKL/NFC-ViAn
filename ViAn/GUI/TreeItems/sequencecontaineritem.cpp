#include "sequencecontaineritem.h"

void SequenceContainerItem::update_text() {
    this->setText(0, BASE_NAME + " [" + QString::number(childCount()) + "]");
}

SequenceContainerItem::SequenceContainerItem() : TreeItem(SEQUENCE_CONTAINER_ITEM) {
    const QIcon icon(":/Icons/add_video");
    setIcon(0, icon);
}

void SequenceContainerItem::remove() {}

void SequenceContainerItem::rename() {}
