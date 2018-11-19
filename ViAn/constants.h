#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

#include <cmath>
#include <string>

class Singleton {
private:
    Singleton();

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    static Singleton* get_instance();

    // Constants
    int LINE_THICKNESS = 2;
    int PAGE_STEP = 10;
};

namespace Constants {
    const int LINE_THICKNESS_MIN = 0;
    const int LINE_THICKNESS_MAX = 10;

    const int PAGE_STEP_MIN = 1;
    const int PAGE_STEP_MAX = 100;


    const double DEGREE_TO_RADIAN_FACTOR = M_PI / 180;

    const int FULL_HD_WIDTH = 1920;
    const int FULL_HD_HEIGHT = 1080;

    const std::string MOTION_DET_STR = "-motion_analysis";
    // "Constants" in analysissettings and in motiondetection

    // savable

    static const int BOOKMARK_THUMBNAIL_HEIGHT = 64;

    const QString NEW_STR = " (NEW!)";

    const int PAN_FACTOR = 20;
    const double ZOOM_STEP = 1.1;


    // enums
    // Save_format - savable
    // list_types - bookmarkwidget
    // Drawing_item - shapeitem
    // item_type - treeitem
}


#endif // CONSTANTS_H
