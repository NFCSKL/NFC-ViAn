#ifndef ZOOMRECTANGLE_H
#define ZOOMRECTANGLE_H

#include "shape.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/core/core.hpp"

class zoomrectangle : public shape {
public:
    zoomrectangle();
    zoomrectangle(QPoint pos);
    void set_start_pos(QPoint pos);
    void area_choosen();
    cv::Rect get_zoom_area();
    void set_zoom_area(int width, int height);
    void draw(QImage &img) override;
    void handle_new_pos(QPoint pos);
private:
    cv::Rect current_zoom_rect = cv::Rect(0, 0, 1280, 720);
};

#endif // ZOOMRECTANGLE_H
