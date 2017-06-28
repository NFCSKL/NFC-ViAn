#include "drawscrollarea.h"

DrawScrollArea::DrawScrollArea(QWidget* parent) : QScrollArea(parent) {

}

void DrawScrollArea::resizeEvent(QResizeEvent *event) {
    emit new_size(this->size());
}
