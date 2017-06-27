#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/core/core.hpp"

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QImage>
#include "overlay.h"
#include "analysisoverlay.h"
#include "Project/Analysis/analysis.h"
#include "zoomer.h"

#include <chrono>

using namespace std;

class video_player : public QThread {
    Q_OBJECT
    cv::VideoCapture capture;
    cv::Mat frame, manipulated_frame;   // Frame is the original frame read video capture

    // Current capture data
    int num_frames;
    int original_height;
    int original_width;
    double frame_rate;

    int new_frame_num;
    int frame_width;
    int frame_height;

    // Interpolation method used when zooming
    int choosen_interpol = cv::INTER_NEAREST;
    double scale_factor = 1;
    cv::Rect zoom_rect;
    Zoomer* zoomer = nullptr;




    double speed_multiplier = DEFAULT_SPEED_MULT;

    std::string file_path;

    bool video_stopped = true;
    bool video_paused = false;
    bool choosing_zoom_area = false;
    bool set_new_frame = false;
    bool slider_moving = false;
    bool choosing_analysis_area = false;

    QMutex* m_mutex;
    QWaitCondition* m_paused_wait;

    AnalysArea* analysis_area = new AnalysArea();
    Analysis m_analysis;

    // Constants for the directions of the rotation.
    int const ROTATE_90 = 0, ROTATE_180 = 1, ROTATE_270 = 2, ROTATE_NONE = 3;
    // The limits of the rotation. This should not include the no-rotaion option.
    int const ROTATE_MIN = 0, ROTATE_MAX = 2;
    // Number of directions.
    int const ROTATE_NUM = 4;
    int rotate_direction = ROTATE_NONE;

    // Contrast, value in range CONTRAST_MIN to CONTRAST_MAX.
    double alpha = 1;
    // Brightness, value in range BRIGHTNESS_MIN to BRIGHTNESS_MAX.
    int beta = 0;

    Overlay* video_overlay;
    AnalysisOverlay* analysis_overlay = new AnalysisOverlay();

    friend class OverlayIntegrationTest;

public:
    video_player(QMutex* mutex, QWaitCondition* paused_wait, QObject* parent = 0);
    ~video_player();
    bool load_video(string filename, Overlay* o);
    bool is_paused();
    bool is_stopped();
    bool is_playing();
    void set_showing_overlay(bool value);
    bool is_showing_overlay();
    bool is_showing_analysis_overlay();
    bool is_showing_analysis_tool();
    QImage get_current_frame_unscaled();
    bool video_open();

    double get_frame_rate();
    int get_num_frames();    
    int get_current_frame_num();
    int get_current_time();
    void set_frame_width(int new_value);
    void set_frame_height(int new_value);
    void set_speed_multiplier(double mult);
    double get_speed_multiplier();
    std::string get_file_name();

    void inc_playback_speed();
    void dec_playback_speed();

    void set_slider_frame(int frame_nbr);
    
    void reset_brightness_contrast();
    void set_contrast(double contrast);
    void set_brightness(int brightness);
    double get_contrast();
    int get_brightness();
    void toggle_overlay();
    void toggle_analysis_overlay();
    void set_overlay_tool(SHAPES shape);
    void set_overlay_colour(QColor colour);
    void undo_overlay();
    void clear_overlay();
    void toggle_analysis_area();
    void invert_analysis_area();
    bool is_including_area();
    void zoom_in();
    void zoom_out();
    void rotate_right();
    void rotate_left();
    int get_video_width();
    int get_video_height();
    std::vector<cv::Point>* get_analysis_area_polygon();
    void clear_analysis_overlay();

    friend class test_video_player;

    const double MAX_SPEED_MULT = 16;
    const double MIN_SPEED_MULT = 1.0/16;
    const double DEFAULT_SPEED_MULT = 1;
    const double SPEED_STEP_MULT = 2;

    // Constants for the limits and the precision of contrast and brightness values.
    const double CONTRAST_MIN = 0.5, CONTRAST_MAX = 5, CONTRAST_DEFAULT = 1, CONTRAST_STEP = 0.01;
    const int CONTRAST_DECIMALS = 2;
    const int BRIGHTNESS_MIN = -100, BRIGHTNESS_MAX = 100, BRIGHTNESS_DEFAULT = 0, BRIGHTNESS_STEP = 1;


signals:
    void capture_frame_size(QSize size);
    void processed_image(const cv::Mat image);
    void update_current_frame(const int frame);
    void frame_count(int value);
    void total_time(int time);

private slots:
    void on_set_playback_frame(int frame_num);
    void on_set_analysis_results(Analysis analysis);

public slots:
    void on_play_video();
    void on_pause_video();
    void on_stop_video();
    void next_frame();
    void previous_frame();
    void set_playback_speed(int speed_steps);
    void set_zoom_rect(QPoint p1, QPoint p2);
    void set_viewport_size(QSize size);
    void fit_screen();
    void on_move_zoom_rect(int x, int y);
protected:
    void run() override;
    void msleep(int ms);

private:
    void read_capture_data();
    void reset();
    void update_frame(int frame_nbr);
    cv::Mat contrast_frame(cv::Mat &src);
    cv::Mat scale_frame(cv::Mat &src);
    void process_frame(bool scale);
    void update_overlay();
    void show_frame();
    void convert_frame(bool scale);
    void scale_position(QPoint &pos);
    void scale_mat(double scale);
    bool limited_frame_dimensions();
    bool set_current_frame_num(int frame_nbr);
};

#endif // VIDEO_PLAYER_H
