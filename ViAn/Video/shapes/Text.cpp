#include "Text.h"

/**
 * @brief Text::Text
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Text::Text(QColor col, QPoint pos, QString strng) : Shape(col, pos) {
    string = strng;
}

/**
 * @brief Text::draw
 * Draws the object on top of the specified QImage.
 * @param img QImage to draw on
 */
void Text::draw(QImage &img) {
    QPainter painter(&img);
    setup_paint_tool(painter);

    QFont font = painter.font() ;
    font.setPointSize(FONT_SIZE);
    painter.setFont(font);

    painter.drawText(draw_end.x(), draw_end.y(), string);
    painter.end();
}

/**
 * @brief Text::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void Text::handle_new_pos(QPoint pos) {
}
