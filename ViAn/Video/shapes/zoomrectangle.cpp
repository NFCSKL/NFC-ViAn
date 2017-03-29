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
 * Coordinates outside the frame are moved to the closest
 * coordinate on the frame.
 * (Overrides the function in the shape class.)
 * @param pos
 */
void zoomrectangle::update_drawing_pos(QPoint pos) {
    // The zoom area has to be inside the video.
    draw_end = bounded_coords(pos);
}

/**
 * @brief zoomrectangle::set_start_pos
 * Used when the user presses the mouse button to start choosing an area.
 * Coordinates outside the frame are moved to the closest
 * coordinate on the frame.
 * @param pos The start position of the object.
 */
void zoomrectangle::set_start_pos(QPoint pos) {
    // The zoom area has to be inside the video.
    draw_start = bounded_coords(pos);
}

/**
 * @brief zoomrectangle::area_choosen
 * Called when the mouse has been released and an area
 * has been choosen. If it is a valid area it's stored
 * as the current zoom level.
 */
void zoomrectangle::choose_area() {
    // Rectangle starts in the top left corner, with positive width and height
    cv::Rect rect(draw_start, draw_end);

    // Zoom limit
    if (rect.width < MINIMUM_ZOOM_SIZE || rect.height < MINIMUM_ZOOM_SIZE) {
        return;
    }

    // Update current zoom level
    current_zoom_rect = rect;
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
    cv::Rect rect(draw_start, draw_end);
    cv::Mat area = frame(rect);
    // CV_8UC3 means 8-bit unsigned 3-channel array.
    cv::Mat coloured_area(area.size(), CV_8UC3, cv::Scalar(125, 125, 125));
    double alpha = 0.6;
    cv::addWeighted(coloured_area, alpha, area, 1.0 - alpha , 0.0, area);
    cv::rectangle(frame, rect, colour, LINE_THICKNESS);
    return frame;
}

/**
 * @brief zoomrectangle::getX
 * @return Returns the x coordinate of the top right corner.
 */
int zoomrectangle::get_x() {
   return current_zoom_rect.x;
}

/**
 * @brief zoomrectangle::getY
 * @return Returns the y coordinate of the top right corner.
 */
int zoomrectangle::get_y() {
    return current_zoom_rect.y;
}

/**
 * @brief zoomrectangle::getWidth
 * @return Returns the width.
 */
int zoomrectangle::get_width() {
    return current_zoom_rect.width;
}

/**
 * @brief zoomrectangle::getHeight
 * @return Returns the height.
 */
int zoomrectangle::get_height() {
    return current_zoom_rect.height;
}

/**
 * @brief zoomrectangle::bounded_coords
 * Returns coordinates closest to the specified
 * position within the video frame, converted
 * to an OpenCV Point.
 * @param pos Coordinate.
 * @return
 */
cv::Point zoomrectangle::bounded_coords(QPoint pos) {
    return cv::Point(std::min(width_video, std::max(0, pos.x())),
                     std::min(height_video, std::max(0, pos.y())));
}
