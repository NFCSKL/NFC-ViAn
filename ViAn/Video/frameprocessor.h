#ifndef FRAMEPROCESSOR_H
#define FRAMEPROCESSOR_H

#include <QObject>
#include <QImage>
#include <QSize>

#include <mutex>
#include <condition_variable>

#include <opencv2/core/core.hpp>
#include <opencv2/videoio/videoio.hpp>

#include "zoomer.h"
#include "framemanipulator.h"
#include "overlay.h"
#include "Video/videoplayer.h"

struct zoomer_settings {
    int video_width = 0;
    int video_height = 0;

    QSize draw_area_size = QSize(100,100);

    QPoint zoom_tl = QPoint(0,0);
    QPoint zoom_br = QPoint(100,100);

    double zoom_factor = 1;

    // Panning
    int x_movement = 0;
    int y_movement = 0;

    // Specific commands
    bool fit = false;
    bool original = false;
};

struct manipulation_settings {
    int brightness = 0;
    double contrast = 1;

    // -1 LEFT(CCW), 0 NONE, 1 RIGHT(CW)
    int rotate = 0;
};

struct overlay_settings {
    Overlay* overlay = nullptr;

    QPoint pos = QPoint(0,0);
    int frame_nr = 0;
    bool mouse_clicked = false;
    bool mouse_released = false;
    bool mouse_moved = false;

    bool undo = false;
    bool redo = false;
    bool clear_drawings = false;

    bool overlay_removed = false;

    SHAPES tool = NONE;
    QColor color = Qt::red;
    QString current_string = "Enter text";
    float current_font_scale = 1;

    bool show_overlay = true;
};

class FrameProcessor : public QObject {
    Q_OBJECT
    cv::Mat m_frame;
    std::atomic_int* m_frame_index;

    std::atomic_int* m_width;
    std::atomic_int* m_height;

    std::atomic_bool* m_new_frame;
    std::atomic_bool* m_new_drawing;
    std::atomic_bool* m_changed;
    std::atomic_bool* m_overlay_changed;
    std::atomic_bool* m_new_video;

    overlay_settings* m_o_settings;
    zoomer_settings* m_z_settings;
    manipulation_settings* m_man_settings;
    video_sync* m_v_sync;

    // Constants for the directions of the rotation.
    int const ROTATE_90 = 0, ROTATE_180 = 1, ROTATE_270 = 2, ROTATE_NONE = 3;
    // The limits of the rotation. This should not include the no-rotaion option.
    int const ROTATE_MIN = 0, ROTATE_MAX = 2;
    // Number of directions.
    int const ROTATE_NUM = 4;
    int m_rotate_direction = ROTATE_NONE;

    Zoomer m_zoomer;
    FrameManipulator m_manipulator;
    Overlay* m_overlay = nullptr;
public:
    FrameProcessor(std::atomic_bool* new_frame, std::atomic_bool* changed,
                   zoomer_settings* z_settings, std::atomic_int* width, std::atomic_int* height,
                   std::atomic_bool* new_video, manipulation_settings* m_settings, video_sync* v_sync,
                   std::atomic_int* frame_index, overlay_settings *o_settings, atomic_bool *overlay_changed);

public slots:
    void check_events(void);
signals:
    void set_scale_factor(double);
    void set_anchor(QPoint);
    void done_processing(cv::Mat frame, int frame_index);
private:
    void process_frame();
    void update_zoomer_settings();
    void update_manipulator_settings();
    void update_overlay_settings();

    void reset_settings();
};

#endif // FRAMEPROCESSOR_H
