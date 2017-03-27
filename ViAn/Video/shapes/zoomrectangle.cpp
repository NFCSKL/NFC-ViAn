#include "zoomrectangle.h"

/**
 * @brief zoomrectangle::zoomrectangle
 */
zoomrectangle::zoomrectangle() : rectangle(QColor(0, 255, 0), QPoint(0, 0)) {
}

/**
 * @brief zoomrectangle::zoomrectangle
 * @param pos Starting point for the new object
 */
zoomrectangle::zoomrectangle(QPoint pos) : rectangle(QColor(0, 255, 0), pos) {
}

/**
 * @brief zoomrectangle::set_start_pos
 * Used when the user presses the mouse button to start choosing an area.
 * @param pos The start position of the object.
 */
void zoomrectangle::set_start_pos(QPoint pos) {
    draw_start = pos;
}

/**
 * @brief zoomrectangle::area_choosen
 * Called when the mouse has been released and an area
 * has been choosen. If it is a valid area it's stored
 * as the current zoom level.
 */
void zoomrectangle::choose_area() {
    // Calculate the the coordinates in realtion to the current zoom level.
    double new_start_x = current_zoom_rect.x + ((double) draw_start.x()/width_video) * current_zoom_rect.width;
    double new_start_y = current_zoom_rect.y + ((double) draw_start.y()/height_video) * current_zoom_rect.height;
    double new_end_x = current_zoom_rect.x + ((double) draw_end.x()/width_video) * current_zoom_rect.width;
    double new_end_y = current_zoom_rect.y + ((double) draw_end.y()/height_video) * current_zoom_rect.height;
    int new_width = new_end_x - new_start_x;
    int new_height = new_end_y - new_start_y;

    // Rectangle starts in the top left corner, with positive width and height
    int use_start_x = std::min(new_start_x, new_end_x);
    int use_start_y = std::min(new_start_y, new_end_y);
    int use_width = std::abs(new_width);
    int use_height = std::abs(new_height);

    // Zoom limit
    if (use_width < 100 || use_height < 100) {
        return;
    }

    // Update current zoom level
    current_zoom_rect.x = use_start_x;
    current_zoom_rect.y = use_start_y;
    current_zoom_rect.width = use_width;
    current_zoom_rect.height = use_height;
}

/**
 * @brief zoomrectangle::get_zoom_area
 * @return Returns the area choosen.
 */
cv::Rect zoomrectangle::get_zoom_area() {
    return current_zoom_rect;
}

/**
 * @brief zoomrectangle::reset_zoom_area
 * Resets the zoom area to the video size.
 */
void zoomrectangle::reset_zoom_area() {
    set_zoom_area(0, 0, width_video, height_video);
}

/**
 * @brief zoomrectangle::set_size
 * Sets the video size to the specified width and
 * height, and sets the zoom area to the specified
 * width and height, starting in (0,0).
 * @param width Width of the video.
 * @param height Height of the video.
 */
void zoomrectangle::set_size(int width, int height) {
    width_video = width;
    height_video = height;
    reset_zoom_area();
}

/**
 * @brief zoomrectangle::set_zoom_area
 * @param x Top left x coordinate of the area.
 * @param y Top left y coordinate of the area.
 * @param width Width of the area.
 * @param height Height of the area.
 */
void  zoomrectangle::set_zoom_area(int x, int y, int width, int height) {
    current_zoom_rect.x = x;
    current_zoom_rect.y = y;
    current_zoom_rect.width = width;
    current_zoom_rect.height = height;
}

/**
 * @brief zoomrectangle::draw
 * Draws the object on top of the specified QImage.
 * @param img QImage to draw on
 */
void zoomrectangle::draw(QImage &img) {

    // Code for drawing with OpenCV library
    /*cv::Rect roi(400, 100, 600, 300);
    cv::Mat part_frame = frame(roi);
    cv::Mat color(part_frame.size(), CV_8UC3, cv::Scalar(0, 125, 125));
    double alpha = 0.6;
    cv::addWeighted(color, alpha, part_frame, 1.0 - alpha , 0.0, part_frame);*/

    QPainter painter(&img);
    setup_paint_tool(painter);
    int width = draw_end.x() - draw_start.x();
    int height = draw_end.y() - draw_start.y();
    painter.setOpacity(0.5);
    painter.fillRect(draw_start.x(), draw_start.y(), width, height, QColor(100,100,100));
    painter.drawRect(draw_start.x(), draw_start.y(), width, height);
    painter.end();
}
