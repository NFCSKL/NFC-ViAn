#include "zoomrectangle.h"

/**
 * @brief zoomrectangle::zoomrectangle
 */
zoomrectangle::zoomrectangle() : shape(QColor(0,255,0), QPoint(0,0)) {
}

/**
 * @brief zoomrectangle::zoomrectangle
 * @param pos Starting point for the new object
 */
zoomrectangle::zoomrectangle(QPoint pos) : shape(QColor(0,255,0), pos) {
}

/**
 * @brief zoomrectangle::set_start_pos
 * @param pos The start position of the object.
 */
void zoomrectangle::set_start_pos(QPoint pos) {
    draw_start = pos;
}

/**
 * @brief zoomrectangle::draw
 * Draws the object on top of the specified QImage.
 * @param img QImage to draw on
 */
void zoomrectangle::draw(QImage &img) {
    QPainter painter(&img);
    setup_paint_tool(painter);
    int width = draw_end.x() - draw_start.x();
    int height = draw_end.y() - draw_start.y();
    painter.setOpacity(0.5);
    painter.fillRect(draw_start.x(), draw_start.y(), width, height, QColor(100,100,100));
    painter.drawRect(draw_start.x(), draw_start.y(), width, height);
    painter.end();
}

/**
 * @brief zoomrectangle::add_drawing_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void zoomrectangle::handle_new_pos(QPoint pos) {
}
