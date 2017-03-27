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
 * @param pos The start position of the object.
 */
void zoomrectangle::set_start_pos(QPoint pos) {
    draw_start = pos;
}

/**
 * @brief zoomrectangle::area_choosen
 * Called when an area has been choosen. If it is a
 * valid area it's stored as the current zoom level.
 */
void zoomrectangle::area_choosen() {
    double newstartx = current_zoom_rect.x + ((double) draw_start.x()/width_video) * current_zoom_rect.width;
    double newstarty = current_zoom_rect.y + ((double) draw_start.y()/height_video) * current_zoom_rect.height;
    double newendx = current_zoom_rect.x + ((double) draw_end.x()/width_video) * current_zoom_rect.width;
    double newendy = current_zoom_rect.y + ((double) draw_end.y()/height_video) * current_zoom_rect.height;

    int new_width = newendx - newstartx;
    int new_height = newendy - newstarty;

    int width = draw_end.x() - draw_start.x();
    int height = draw_end.y() - draw_start.y();

    std::cout <<"X: "<<newstartx<< "\n";
    std::cout <<"Y: "<<newstarty<< "\n";
    std::cout <<"W: "<<width<< "\n";
    std::cout <<"H: "<<height<< "\n";
    std::cout <<"W: "<<new_width<< "\n";
    std::cout <<"H: "<<new_height<< "\n";
    std::cout << "\n";

    if (width < 10 || height < 10) {
        std::cout<<"not zoomed\n";
        return;
    }

    current_zoom_rect.x = (int) newstartx;
    current_zoom_rect.y = (int) newstarty;
    current_zoom_rect.width = width;
    current_zoom_rect.height = height;
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
void  zoomrectangle::reset_zoom_area() {
    current_zoom_rect.x = 0;
    current_zoom_rect.y = 0;
    current_zoom_rect.width = width_video;
    current_zoom_rect.height = height_video;
}

/**
 * @brief zoomrectangle::reset_zoom_area
 * Resets the zoom area to the specified width and
 * height, starting in (0,0).
 * @param width Width of the area.
 * @param height Height of the area.
 */
void  zoomrectangle::reset_zoom_area(int width, int height) {
    current_zoom_rect.x = 0;
    current_zoom_rect.y = 0;
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
