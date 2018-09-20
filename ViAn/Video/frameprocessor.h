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
    QSize preview_window_size = QSize(100, 50);

    QPoint zoom_area_tl = QPoint(0,0);
    QPoint zoom_area_br = QPoint(100,100);
    QPoint center = QPoint(50,50);
    QPoint anchor = QPoint(0,0);

    int rotation = 0;

    bool set_state = false;

    double zoom_factor = 1;
    double zoom_step = 1;

    int interpolation = cv::INTER_NEAREST;

    // Panning
    int x_movement = 0;
    int y_movement = 0;

    // Specific commands
    bool fit = false;
    bool original = false;
    bool do_point_zoom = false;
    bool has_new_zoom_area{false};
    bool skip_frame_refresh{false};
};

/**
 * @brief The manipulation_settings struct
 * Manipulation settings need to be synchronized,
 * use v_sync_lock
 */
struct manipulation_settings {
    int brightness = 0;
    double contrast = 1;
    bool update_state = true;

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
    int frame = 0;
    bool mouse_double_clicked = false;
    bool mouse_clicked = false;
    bool mouse_released = false;
    bool mouse_moved = false;
    bool mouse_scroll = false;
    bool shift_modifier = false;
    bool ctrl_modifier = false;

    bool update_text = false;
    bool clear_drawings = false;
    bool delete_drawing = false;
    bool show_overlay = true;
    bool overlay_removed = false;
    bool right_click = false;
    bool set_current_drawing = false;
    bool create_text = false;

    SHAPES tool = ZOOM;
    QColor color = Qt::red;
    QString text = "";
    QString current_string = "Enter text";
    float current_font_scale = 1;
    Shapes* shape = nullptr;

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
    cv::Size m_unrotated_size{};
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
    std::atomic_bool* m_new_frame_video;

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

    static const int DEGREES_0;
    static const int DEGREES_90;
    static const int DEGREES_180;
    static const int DEGREES_270;

    // Used to zoom and scale frame
    Zoomer m_zoomer;
    // Used to adjust contrast and brightness of frame
    FrameManipulator m_manipulator;
    // Overlay to draw on frame
    Overlay* m_overlay = nullptr;

    bool has_new_zoom_state{false};
    std::atomic_bool* m_abort;
public:
    FrameProcessor(std::atomic_bool* new_frame, std::atomic_bool* changed,
                   zoomer_settings* z_settings, std::atomic_int* width, std::atomic_int* height,
                   std::atomic_bool* new_frame_video, manipulation_settings* m_settings, video_sync* v_sync,
                   std::atomic_int* frame_index, overlay_settings *o_settings, std::atomic_bool *overlay_changed, std::atomic_bool *abort);
    ~FrameProcessor();

public slots:
    void check_events(void);
signals:
    void set_scale_factor(double);
    void set_anchor(QPoint);
    void set_zoom_state(QPoint, double, int);
    void set_play_btn(bool);
    void set_bri_cont(int, double);
    void done_processing(cv::Mat org_frame, cv::Mat mod_frame, int frame_index);
    void zoom_preview(cv::Mat preview_frame);
private:
    void process_frame();
    void update_zoomer_settings();
    void update_manipulator_settings();
    void update_overlay_settings();
    void update_rotation(const int& rotation);

    void reset_settings();
    void load_zoomer_state();
    void emit_zoom_data();
};

#endif // FRAMEPROCESSOR_H
