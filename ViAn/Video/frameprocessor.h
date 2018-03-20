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

/**
 * @brief The zoomer_settings struct
 * Zoomer settings changes need to be synchronized,
 * use v_sync_lock
 */
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

/**
 * @brief The manipulation_settings struct
 * Manipulation settings need to be synchronized,
 * use v_sync_lock
 */
struct manipulation_settings {
    int brightness = 0;
    double contrast = 1;

    // -1 LEFT(CCW), 0 NONE, 1 RIGHT(CW)
    int rotate = 0;
};

/**
 * @brief The overlay_settings struct
 * Overlay settings, changes need to be synchronized
 * with v_sync_lock
 */
struct overlay_settings {
    Overlay* overlay = nullptr;

    QPoint pos = QPoint(0,0);
    int frame_nr = 0;
    bool mouse_clicked = false;
    bool mouse_released = false;
    bool mouse_moved = false;
    bool mouse_scroll = false;

    bool undo = false;
    bool redo = false;
    bool clear_drawings = false;
    bool delete_drawing = false;
    bool show_overlay = true;
    bool overlay_removed = false;
    bool right_click = false;

    SHAPES tool = NONE;
    QColor color = Qt::red;
    QString current_string = "Enter text";
    float current_font_scale = 1;

};
/**
 * @brief The FrameProcessor class
 * Image processing class
 * Input : Original frame
 * Output : Edited frame, zoomed, annotated, scaled etc
 * see settings structs above
 */
class FrameProcessor : public QObject {
    Q_OBJECT
    cv::Mat m_frame;
    std::atomic_int* m_frame_index;

    /**
     * @brief m_width, m_height
     * Height and width of video
     *
     * OBS! If loading video crashes, it is likely
     * due to these two settings being set incorrectly
     * or unsynchronized with load. As it causes
     * settingsprocessing race condition.
     */
    std::atomic_int* m_width;
    std::atomic_int* m_height;

    // New Frame input by video player
    std::atomic_bool* m_new_frame;
    // Drawing input from user
    std::atomic_bool* m_new_drawing;
    // Zoomer or manipulator settings changed by user
    std::atomic_bool* m_changed;
    // Overlay changed by user
    std::atomic_bool* m_overlay_changed;
    // New video loaded by video player
    std::atomic_bool* m_new_video;

    /**
     *  Input settings,
     *  can be changed dynamically,
     *  needs synchronization throght v_sync_lock
     */
    overlay_settings* m_o_settings;
    zoomer_settings* m_z_settings;
    manipulation_settings* m_man_settings;

    // VIDEO SYNC
    video_sync* m_v_sync;

    // Constants for the directions of the rotation.
    int const ROTATE_90 = 0, ROTATE_180 = 1, ROTATE_270 = 2, ROTATE_NONE = 3;
    // The limits of the rotation. This should not include the no-rotaion option.
    int const ROTATE_MIN = 0, ROTATE_MAX = 2;
    // Number of directions.
    int const ROTATE_NUM = 4;
    int m_rotate_direction = ROTATE_NONE;

    // Used to zoom and scale frame
    Zoomer m_zoomer;
    // Used to adjust contrast and brightness of frame
    FrameManipulator m_manipulator;
    // Overlay to draw on frame
    Overlay* m_overlay = nullptr;


public:
    FrameProcessor(std::atomic_bool* new_frame, std::atomic_bool* changed,
                   zoomer_settings* z_settings, std::atomic_int* width, std::atomic_int* height,
                   std::atomic_bool* new_video, manipulation_settings* m_settings, video_sync* v_sync,
                   std::atomic_int* frame_index, overlay_settings *o_settings, atomic_bool *overlay_changed);
    ~FrameProcessor();
    bool loop = true;
public slots:
    void check_events(void);
signals:
    void set_scale_factor(double);
    void set_anchor(QPoint);
    void done_processing(cv::Mat org_frame, cv::Mat mod_frame, int frame_index);
private:
    void process_frame();
    void update_zoomer_settings();
    void update_manipulator_settings();
    void update_overlay_settings();

    void reset_settings();
};

#endif // FRAMEPROCESSOR_H
