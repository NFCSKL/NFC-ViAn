#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QSize>
#include <QString>
#include <QStandardPaths>

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
    int FLOATING_WIDGET_MAX = 3;
    QSize PROJ_THUMBNAIL_SIZE = QSize(20,20);
};

namespace Constants {
    const int LINE_THICKNESS_MIN = 0;
    const int LINE_THICKNESS_MAX = 10;

    const int PAGE_STEP_MIN = 1;
    const int PAGE_STEP_MAX = 100;

    const int FLOATING_WIDGET_LIMIT_MIN = 0;
    const int FLOATING_WIDGET_LIMIT_MAX = 20;

    const unsigned int PROJ_THUMBNAIL_MIN = 10;
    const unsigned int PROJ_THUMBNAIL_MAX = 120;


    const int FULL_HD_WIDTH = 1920;
    const int FULL_HD_HEIGHT = 1080;
    const int SIZE_MULTIPLIER = 40;
    const int VIDEO_WGT_WIDTH = 16; // 16:9 aspect ratio
    const int VIDEO_WGT_HEIGHT = 9;

    const std::string MOTION_DET_STR = "-motion_analysis";
    const QString NEW_STR = " (NEW!)";
    const int BOOKMARK_THUMBNAIL_HEIGHT = 64;

    const int PAN_FACTOR = 20;
    const double ZOOM_STEP = 1.1;

    const double DEGREE_TO_RADIAN_FACTOR = M_PI / 180;
    const int DEGREE_MIN = 0;
    const int DEGREE_90 = 90;
    const int DEGREE_180 = 180;
    const int DEGREE_270 = 270;
    const int DEGREE_MAX = 360;

    const double DOUBLE_TO_INT = 100;
    const int ZOOM_LABEL_MIN = 1;
    const int ZOOM_LABEL_MAX = 10000;
    const int FIVE_SEC = 5000;

    const QString DEFAULT_PATH = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ViAn Projects";
    const QString DEFAULT_TEMP_PATH = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    const QString BOOKMARK_FOLDER = "Bookmarks/";
    const QString THUMBNAIL_FOLDER = "_thumbnails/";
    const QString STILLS_FOLDER = "Stills/";
    const QString SEQUENCE_FOLDER = "Sequences/";
    const QString RECENT_FILE_NAME = "recent_projects";
    const QString RECENT_FILE_PATH = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/ViAn/";

    const int RECENT_MAX = 10;

    // Constants describing the limit and precision of the font scale value.
    const double FONT_SCALE_MIN = 0.5;
    const double FONT_SCALE_MAX = 5.0;
    const double FONT_SCALE_STEP = 0.1;
    const int FONT_SCALE_DECIMALS = 1;

    // Constants for the limits and the precision of contrast and brightness values.
    const double CONTRAST_MIN = 0.5, CONTRAST_MAX = 5, CONTRAST_DEFAULT = 1, CONTRAST_STEP = 0.01;
    const int BRIGHTNESS_MIN = -100, BRIGHTNESS_MAX = 100, BRIGHTNESS_DEFAULT = 0, BRIGHTNESS_STEP = 1;
    const double GAMMA_MIN = 0.01, GAMMA_MAX = 4, GAMMA_DEFAULT = 1, GAMMA_STEP = 0.01;

    const unsigned int THUMBNAIL_SIZE = 60;
}


#endif // CONSTANTS_H
