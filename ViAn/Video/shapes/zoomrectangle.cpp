#include "zoomrectangle.h"

/**
 * @brief zoomrectangle::zoomrectangle
 */
zoomrectangle::zoomrectangle() : rectangle(QColor(0, 255, 0), QPoint(0, 0)) {
}

/**
 * @brief zoomrectangle::zoomrectangle
 * @param pos Starting point for the new object.
 */
zoomrectangle::zoomrectangle(QPoint pos) : rectangle(QColor(0, 255, 0), pos) {
}

/**
 * @brief shape::update_drawing_pos
 * Updates the position of the end point of the drawing.
 * (Overrides the function in the shape class.)
 * @param pos
 */
void zoomrectangle::update_drawing_pos(QPoint pos) {
    // The zoom area has to be inside the video.
    int x = std::min(width_video, std::max(0, pos.x()));
    int y = std::min(height_video, std::max(0, pos.y()));
    draw_end.setX(x);
    draw_end.setY(y);
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
    /*double new_start_x = current_zoom_rect.x + ((double) draw_start.x()/width_video) * current_zoom_rect.width;
    double new_start_y = current_zoom_rect.y + ((double) draw_start.y()/height_video) * current_zoom_rect.height;
    double new_end_x = current_zoom_rect.x + ((double) draw_end.x()/width_video) * current_zoom_rect.width;
    double new_end_y = current_zoom_rect.y + ((double) draw_end.y()/height_video) * current_zoom_rect.height;
    int new_width = new_end_x - new_start_x;
    int new_height = new_end_y - new_start_y;*/

    // Rectangle starts in the top left corner, with positive width and height
    /*int use_start_x = std::min(new_start_x, new_end_x);
    int use_start_y = std::min(new_start_y, new_end_y);
    int use_width = std::abs(new_width);
    int use_height = std::abs(new_height);*/

    int use_start_x = std::min(draw_start.x(), draw_end.x());
    int use_start_y = std::min(draw_start.y(), draw_end.y());
    int use_width = std::abs(draw_end.x() - draw_start.x());
    int use_height = std::abs(draw_end.x() - draw_start.x());

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
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat zoomrectangle::draw(cv::Mat &frame) {
    int width = std::abs(draw_end.x() - draw_start.x());
    int height = std::abs(draw_end.y() - draw_start.y());
    int x = std::min(draw_start.x(), draw_end.x());
    int y = std::min(draw_start.y(), draw_end.y());
    cv::Rect roi(x, y, width, height);
    cv::Mat part_frame = frame(roi);
    cv::Mat color(part_frame.size(), CV_8UC3, cv::Scalar(125, 125, 125));
    double alpha = 0.6;
    cv::addWeighted(color, alpha, part_frame, 1.0 - alpha , 0.0, part_frame);
    cv::rectangle(frame, roi, qcolor2scalar(colour), LINE_THICKNESS);
    return frame;
}

/**
 * @brief zoomrectangle::getX
 * @return Returns the x coordinate of the top right corner.
 */
int zoomrectangle::getX() {
   return current_zoom_rect.x;
}

/**
 * @brief zoomrectangle::getY
 * @return Returns the y coordinate of the top right corner.
 */
int zoomrectangle::getY() {
    return current_zoom_rect.y;
}

/**
 * @brief zoomrectangle::getWidth
 * @return Returns the width.
 */
int zoomrectangle::getWidth() {
    return current_zoom_rect.width;
}

/**
 * @brief zoomrectangle::getHeight
 * @return Returns the height.
 */
int zoomrectangle::getHeight() {
    return current_zoom_rect.height;
}
