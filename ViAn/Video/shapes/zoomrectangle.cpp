#include "zoomrectangle.h"
#include <QPoint>
/**
 * @brief ZoomRectangle::ZoomRectangle
 */
ZoomRectangle::ZoomRectangle() : Rectangle(QColor(0, 255, 0), QPoint(0, 0)) {
}

/**
 * @brief ZoomRectangle::ZoomRectangle
 * @param pos Starting point for the new object.
 */
ZoomRectangle::ZoomRectangle(QPoint pos) : Rectangle(QColor(0, 255, 0), pos) {
}

/**
 * @brief ZoomRectangle::~ZoomRectangle
 */
ZoomRectangle::~ZoomRectangle() {
}

/**
 * @brief ZoomRectangle::update_drawing_pos
 * Updates the position of the end point of the drawing.
 * Coordinates outside the frame are moved to the closest
 * coordinate on the frame.
 * (Overrides the function in the shape class.)
 * @param pos
 */
void ZoomRectangle::update_drawing_pos(QPoint pos) {
    // The zoom area has to be inside the video.
    cv::Point bounded_pos = bounded_coords(pos);
    // Calculate the signed ratio between the choosen area and the video.
    double width_ratio = (double) (bounded_pos.x - draw_start.x)/width_video;
    double height_ratio = (double) (bounded_pos.y - draw_start.y)/height_video;
    // Set the area to follow the smallest ratio.
    if (std::abs(width_ratio) >= std::abs(height_ratio)) {
        // Need to keep the original sign when forcing the ratio
        draw_end.x = draw_start.x + std::copysign(width_video * height_ratio, width_ratio);
        draw_end.y = bounded_pos.y;
    } else {
        draw_end.x = bounded_pos.x;
        // Need to keep the original sign when forcing the ratio
        draw_end.y = draw_start.y + std::copysign(height_video * width_ratio, height_ratio);
    }
}

/**
 * @brief ZoomRectangle::reset_pos
 * Resets the start and end mouse positions.
 */
void ZoomRectangle::reset_pos() {
    draw_start.x = 0;
    draw_start.y = 0;
    draw_end.x = 0;
    draw_end.y = 0;
}

/**
 * @brief ZoomRectangle::set_start_pos
 * Used when the user presses the mouse button to start choosing an area.
 * Coordinates outside the frame are moved to the closest
 * coordinate on the frame.
 * @param pos The start position of the object.
 */
void ZoomRectangle::set_start_pos(QPoint pos) {
    // The zoom area has to be inside the video.
//    draw_start = bounded_coords(pos);
    draw_start = cv::Point(pos.x(), pos.y());
}

/**
 * @brief ZoomRectangle::choose_area
 * Called when the mouse has been released and an area
 * has been choosen. If it is a valid area it's stored
 * as the current zoom level.
 */
void ZoomRectangle::choose_area() {
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
 * @brief ZoomRectangle::get_zoom_area
 * @return Returns the area choosen.
 */
cv::Rect ZoomRectangle::get_zoom_area() {
    return current_zoom_rect;
}

/**
 * @brief ZoomRectangle::reset_zoom_area
 * Resets the zoom area to the video size.
 */
void ZoomRectangle::reset_zoom_area() {
    set_zoom_area(0, 0, width_video, height_video);
}

/**
 * @brief ZoomRectangle::set_size
 * Sets the video size to the specified width and
 * height, and sets the zoom area to the specified
 * width and height, starting in (0,0).
 * @param width Width of the video.
 * @param height Height of the video.
 */
void ZoomRectangle::set_size(int width, int height) {
    width_video = width;
    height_video = height;
    reset_zoom_area();
}

/**
 * @brief ZoomRectangle::set_zoom_area
 * @param x Top left x coordinate of the area.
 * @param y Top left y coordinate of the area.
 * @param width Width of the area.
 * @param height Height of the area.
 */
void  ZoomRectangle::set_zoom_area(int x, int y, int width, int height) {
    current_zoom_rect.x = x;
    current_zoom_rect.y = y;
    current_zoom_rect.width = width;
    current_zoom_rect.height = height;
}

/**
 * @brief ZoomRectangle::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat ZoomRectangle::draw(cv::Mat &frame) {
    cv::Rect rect(draw_start, draw_end);
    cv::Mat area = frame(rect);
    // CV_8UC3 means 8-bit unsigned 3-channel array.
    cv::Mat coloured_area(area.size(), CV_8UC3, cv::Scalar(125, 125, 125));
    cv::addWeighted(coloured_area, ALPHA, area, 1.0 - ALPHA , 0.0, area);
    cv::rectangle(frame, rect, color, LINE_THICKNESS);
    return frame;
}

/**
 * @brief ZoomRectangle::get_x
 * @return Returns the x coordinate of the top right corner of the choosen zoom area.
 */
int ZoomRectangle::get_x() {
   return current_zoom_rect.x;
}

/**
 * @brief ZoomRectangle::get_y
 * @return Returns the y coordinate of the top right corner of the choosen zoom area.
 */
int ZoomRectangle::get_y() {
    return current_zoom_rect.y;
}

/**
 * @brief ZoomRectangle::get_width
 * @return Returns the width of the choosen zoom area.
 */
int ZoomRectangle::get_width() {
    return current_zoom_rect.width;
}

/**
 * @brief ZoomRectangle::get_height
 * @return Returns the height of the choosen zoom area.
 */
int ZoomRectangle::get_height() {
    return current_zoom_rect.height;
}

/**
 * @brief ZoomRectangle::bounded_coords
 * Returns coordinates closest to the specified
 * position within the video frame, converted
 * to an OpenCV Point.
 * @param pos Coordinate.
 * @return
 */
cv::Point ZoomRectangle::bounded_coords(QPoint pos) {
    return cv::Point(std::min(width_video, std::max(0, pos.x())),
                     std::min(height_video, std::max(0, pos.y())));
}
