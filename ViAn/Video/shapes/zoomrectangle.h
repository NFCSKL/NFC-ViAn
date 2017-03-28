#ifndef ZOOMRECTANGLE_H
#define ZOOMRECTANGLE_H

#include "rectangle.h"
#include <algorithm>

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
    void choose_area();
    cv::Rect get_zoom_area();
    void reset_zoom_area();
    void set_size(int width, int height);
    void set_zoom_area(int x, int y, int width, int height);
    void draw(QImage &img) override;
private:
    int width_video = 360;
    int height_video = 240;
    cv::Rect current_zoom_rect = cv::Rect(0, 0, 360, 240);
};

#endif // ZOOMRECTANGLE_H
