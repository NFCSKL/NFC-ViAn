#ifndef ZOOMRECTANGLE_H
#define ZOOMRECTANGLE_H

#include "rectangle.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/core/core.hpp"

class zoomrectangle : public rectangle {
public:
    zoomrectangle();
    zoomrectangle(QPoint pos);
    void set_start_pos(QPoint pos);
    void update_drawing_pos(QPoint pos);
    void choose_area();
    cv::Rect get_zoom_area();
    void reset_zoom_area();
    void set_size(int width, int height);
    void set_zoom_area(int x, int y, int width, int height);
    cv::Mat draw(cv::Mat &frame) override;
    int get_x();
    int get_y();
    int get_width();
    int get_height();
private:
    int MINIMUM_ZOOM_SIZE = 100;
    cv::Point bounded_coords(QPoint pos);
    int width_video = 360;
    int height_video = 240;
    cv::Rect current_zoom_rect = cv::Rect(0, 0, 360, 240);
};

#endif // ZOOMRECTANGLE_H
