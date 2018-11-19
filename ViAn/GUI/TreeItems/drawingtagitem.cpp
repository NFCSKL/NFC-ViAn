#include "drawingtagitem.h"

#include"Project/Analysis/tag.h"

DrawingTagItem::DrawingTagItem(Tag *tag) : TreeItem(DRAWING_TAG_ITEM) {
    m_tag = tag;
    setText(0, QString::fromStdString(tag->get_name()));
    const QIcon draw_tag_icon("../Vian/Icons/pen.png");
    setIcon(0, draw_tag_icon);
}

DrawingTagItem::~DrawingTagItem() {
}

void DrawingTagItem::remove() {}

void DrawingTagItem::rename() {
    m_tag->m_name = text(0).toStdString();
}

Tag *DrawingTagItem::get_tag() {
    return m_tag;
}
