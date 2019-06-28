#include "drawingtagitem.h"

#include"Project/Analysis/tag.h"

DrawingTagItem::DrawingTagItem(Tag *tag) : TreeItem(DRAWING_TAG_ITEM) {
    m_tag = tag;
    setText(0, tag->get_name());
    const QIcon draw_tag_icon(":/Icons/pen.png");
    setIcon(0, draw_tag_icon);
}

DrawingTagItem::~DrawingTagItem() {
}

void DrawingTagItem::remove() {}

void DrawingTagItem::rename() {
    m_tag->m_name = text(0);
}

Tag *DrawingTagItem::get_tag() {
    return m_tag;
}
