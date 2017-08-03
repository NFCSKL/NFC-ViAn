#include "video_player.h"
#include <string>
#include <iostream>
#include "GUI/mainwindow.h"
#include <thread>
#include <QWaitCondition>
#include <QDesktopWidget>
#include <qpainter.h>
#include <qdebug.h>
#include <QTime>
#include "frameprocessor.h"


using namespace std;
using namespace cv;

/**
 * @brief video_player::video_player
 * @param mutex
 * @param paused_wait
 * @param label The QLabel where the frame is shown.
 * @param parent
 */
video_player::video_player(QMutex* mutex, QWaitCondition* paused_wait, QObject* parent) : QThread(parent) {
    m_mutex = mutex;
    m_paused_wait = paused_wait;
    zoomer = new Zoomer();
    manipulator = new FrameManipulator();
}

/**
 * @brief video_player::~video_player
 */
video_player::~video_player() {
    delete analysis_area;
    delete zoomer;
    delete manipulator;
    capture.release();
}

/**
 * @brief video_player::load_video
 * This method loads a video from file.
 * @param filename
 * @param o Overlay associated to the video
 * @return whether video is loaded
 */
bool video_player::load_video(string filename, Overlay* o) {
    video_overlay = o;
    reset();
    capture.open(filename);

    if (capture.isOpened()) {
        read_capture_data();
        file_path = filename;
        emit frame_count(num_frames - 1);
        emit total_time(int(num_frames / frame_rate));
        emit capture_frame_size(QSize(original_width, original_height));
        check_last_frame();
        video_paused = true;
        video_stopped = false;
        zoomer->set_frame_size(cv::Size(original_width, original_height));
        return true;
    } else {
        return false;
    }
}

/**
 * @brief video_player::read_capture_data
 * Reads various data from the video capture object
 */
void video_player::read_capture_data() {
    frame_rate = capture.get(CV_CAP_PROP_FPS);
    num_frames = capture.get(CAP_PROP_FRAME_COUNT);
    original_width = capture.get(CAP_PROP_FRAME_WIDTH);
    original_height = capture.get(CAP_PROP_FRAME_HEIGHT);
}

/**
 * @brief video_player::reset
 * Resets the state of the video player
 * Should be used when loading a new video
 */
void video_player::reset() {
    if (capture.isOpened()) capture.release();
    speed_multiplier = DEFAULT_SPEED_MULT;
    rotate_direction = ROTATE_NONE;
    zoomer->reset();
    manipulator->reset();
}

/**
 * @brief video_player::run
 * This function is called whenever the thread is started or put out of sleep.
 * It houses the main loop for fetching frames from the currently played
 * video file and sending them to the GUI.
 */
void video_player::run()  {
    int delay = (1000/frame_rate);
    bool is_end = false;
    while (!video_stopped) {
        // The video capture object can be modified from outside this thread
        // Thus the capture.read cannot be part of the loop condition
        const clock_t begin_time = std::clock();
        frame_lock.lock();
        is_end = !capture.read(frame);
        frame_lock.unlock();
        if (is_end) break;

        // Time the frame processing time and adjust for a smoother playback
        process_frame();
        int conversion_time = int((std::clock()-begin_time)*1000.0 /CLOCKS_PER_SEC);
        if (delay - conversion_time > 0) {
            this->msleep(delay - conversion_time);
        }

        // Waits for the video to be resumed
        m_mutex->lock();
        if (video_paused) {
            m_paused_wait->wait(m_mutex);
            video_paused = false;
        }
        m_mutex->unlock();
    }
    video_stopped = true;
    video_paused = false;
    set_playback_pos(0);
    process_frame();
}

/**
 * @brief video_player::process_frame
 * Manipulates the current frame according to the current program settings
 */
void video_player::process_frame() {
    // Copy the frame, so that we don't alter the original frame (which will be reused next draw loop).
    QMutexLocker locker(&frame_lock);
    manipulated_frame.release();
    manipulated_frame = frame.clone();
//    processed_frame = analysis_overlay->draw_overlay(processed_frame, get_current_frame_num());
//    processed_frame = video_overlay->draw_overlay(processed_frame, get_current_frame_num());

    // Rotates the frame, according to the choosen direction.
    // If direction is in the valid range the frame is rotated.
    if (ROTATE_MIN <= rotate_direction && rotate_direction <= ROTATE_MAX) {
        cv::rotate(manipulated_frame, manipulated_frame, rotate_direction);
    }

    // Scales the frame
    if (zoomer->get_scale_factor() != 1) zoomer->scale_frame(manipulated_frame);
    emit scale_factor(zoomer->get_scale_factor());

    // Applies brightness and contrast
    manipulator->apply(manipulated_frame);

    video_overlay->draw_overlay(manipulated_frame, get_current_frame_num());
    // Emit manipulated frame and current frame number
    emit processed_image(manipulated_frame.clone());
    emit update_current_frame(capture.get(CV_CAP_PROP_POS_FRAMES) - 1);
}

/**
 * @brief video_player::zoom_out
 * Slot function for zooming out
 */
void video_player::zoom_out() {
    frame_lock.lock();
    zoomer->set_scale_factor(zoomer->get_scale_factor() * ZOOM_OUT_FACTOR);
    frame_lock.unlock();
    process_frame();
}

/**
 * @brief video_player::fit_screen
 * Slot function. Will fit the current video to window size
 */
void video_player::fit_screen() {
    frame_lock.lock();
    zoomer->fit_viewport();
    frame_lock.unlock();
    process_frame();
}

Overlay* video_player::get_overlay() {
    return video_overlay;
}

/**
 * @brief video_player::on_move_zoom_rect
 * Slot function. Called when the video is moved
 * @param x movement on the x-axis
 * @param y movement on the y-axis
 */
void video_player::on_move_zoom_rect(int x, int y) {
    frame_lock.lock();
    zoomer->move_zoom_rect(x, y);
    frame_lock.unlock();
    process_frame();

}

/**
 * @brief video_player::set_zoom_rect
 * Slot function. Sets the zoom rectangle in the zoomer
 * @param p1 top-left corner of the rectangle
 * @param p2 bottom-right corner of the rectangle
 */
void video_player::set_zoom_rect(QPoint p1, QPoint p2) {
    frame_lock.lock();
    zoomer->set_zoom_rect(p1, p2);
    frame_lock.unlock();
    process_frame();
}

/**
 * @brief video_player::set_viewport_size
 * @param size
 */
void video_player::set_viewport_size(QSize size) {
    frame_lock.lock();
    if (zoomer != nullptr) zoomer->set_viewport_size(size);
    frame_lock.unlock();
}

/**
 * @brief video_player::set_playback_pos
 * Sets the playback position in frames
 * @param pos the frame number
 */
void video_player::set_playback_pos(int pos) {
    if (pos < 0 || pos > num_frames - 1) return;
    if (video_paused) {
        // The playback is paused and thus the playback thread is sleeping
        // Force frame processing in another thread to prevent GUI lag
        // Ignore position update if already processing
//        if (processing_thread != nullptr && processing_thread->isRunning()) return;

//        processing_thread = new QThread;
//        FrameProcessor* processor = new FrameProcessor(frame, rotate_direction, pos, manipulator, zoomer, &frame_lock, &capture);
//        processor->moveToThread(processing_thread);

//        connect(processing_thread, &QThread::finished, processor, &FrameProcessor::deleteLater);
//        connect(processor, &FrameProcessor::done, processing_thread, &QThread::quit);
//        connect(processing_thread, &QThread::started, processor, &FrameProcessor::process);
//        connect(processor, SIGNAL(done_processing(cv::Mat)), this, SIGNAL(processed_image(cv::Mat)));
//        connect(processor, SIGNAL(frame_num(int)), this, SIGNAL(update_current_frame(int)));
//        processing_thread->start();
    } else {
        frame_lock.lock();
        capture.set(CV_CAP_PROP_POS_FRAMES, pos);   // Set next frame to be read
        capture.read(frame);                        // Read it
        frame_lock.unlock();
    }
    process_frame(); //Blink blink
}

/**
 * @brief video_player::msleep
 * This method is used to make the video player thread sleep the correct amount
 * of time between fetching frames from source video.
 * @param delay
 */
void video_player::msleep(int delay) {
    std::chrono::milliseconds dura((int)(delay*this->speed_multiplier));
    std::this_thread::sleep_for(dura);
}

/**
 * @brief video_player::is_paused
 * Returns a boolean value representing whether the currently played video is paused.
 * @return if the video is paused or not
 */
bool video_player::is_paused() {
    return video_paused;
}

/**
 * @brief video_player::is_stopped
 * Returns a boolean value representing whether the currently played video is stopped.
 * @return
 */
bool video_player::is_stopped() {
    return video_stopped;
}

/**
 * @brief video_player::is_playing
 * Returns a boolean value representing whether the currently played video is playing.
 * @return
 */
bool video_player::is_playing(){
    return !(video_stopped || video_paused);
}

/**
 * @brief video_player::set_showing_overlay
 * @param value
 * Sets the showing/not showing state.
 */
void video_player::set_showing_overlay(bool value) {
    video_overlay->set_showing_overlay(value);
}

/**
 * @brief video_player::is_showing_overlay
 * @return Returns true if the overlay tool is showing, else false.
 */
bool video_player::is_showing_overlay() {
    return video_overlay->is_showing_overlay();
}

/**
 * @brief video_player::is_showing_analysis_tool
 * @return Returns true if the analysis area tool is showing, else false.
 */
bool video_player::is_showing_analysis_tool() {
    return choosing_analysis_area;
}

/**
 * @brief video_player::get_frame_rate
 * @return the frame rate
 */
double video_player::get_frame_rate() {
    return frame_rate;
}

/**
 * @brief video_player::get_num_frames
 * @return number of frames in video file
 */
int video_player::get_num_frames() {
    return num_frames;
}

/**
 * @brief video_player::get_current_frame_num
 * @return The number of the currently read frame (0-based index).
 */
int video_player::get_current_frame_num() {
    // capture.get() gives the number of the frame to be read, hence the compensation of -1.
    return capture.get(CV_CAP_PROP_POS_FRAMES) - 1;
}

/**
 * @brief video_player::get_current_time
 * @return Returns the current time in the clip in milliseconds.
 */
int video_player::get_current_time() {
    return capture.get(CV_CAP_PROP_POS_MSEC);
}

/**
 * @brief video_player::set_frame_width
 * @param new_value
 */
void video_player::set_frame_width(int new_value) {
    frame_width = new_value;
}

/**
 * @brief video_player::set_frame_height
 * @param new_value
 */
void video_player::set_frame_height(int new_value) {
    frame_height = new_value;
}

/**
 * @brief video_player::next_frame
 * Moves the playback one frame forward.
 */
void video_player::next_frame() {
    set_playback_pos(get_current_frame_num() + 1);
}

/**
 * @brief video_player::previous_frame
 * Moves the playback one frame backward.
 */
void video_player::previous_frame() {
    set_playback_pos(get_current_frame_num() - 1);
}

/**
 * @brief on_play_video
 * Slot function to be used from the GUI thread
 * Sets the paused and stopped bools to false
 */
void video_player::on_play_video() {
    video_paused = false;
    video_stopped = false;
    if (!isRunning()) {
        start();
    }
}

/**
 * @brief on_pause_video
 * Slot function to be used from the GUI thread
 * Sets the paused bool to true
 */
void video_player::on_pause_video() {
    video_paused = true;
}

/**
 * @brief on_stop_video
 * Slot function to be used from the GUI thread
 * Sets the stopped bool to true and the paused bool to false.
 * Sets playback frame to the start of the video and updates the GUI.
 */
void video_player::on_stop_video() {
    video_stopped = true;
    video_paused = false;
}

/**
 * @brief video_player::check_last_frame
 * Checks if the last frame is empty.
 */
void video_player::check_last_frame() {
    cv::Mat last;
    capture.set(CV_CAP_PROP_POS_FRAMES, num_frames - 1);
    capture.read(last);
    if (last.empty()) {
        num_frames -= 1;
    }
    capture.set(CV_CAP_PROP_POS_FRAMES, 0);
}

/**
 * @brief video_player::update_overlay
 * Updates the current frame if the video is loaded and paused.
 */
void video_player::update_overlay() {
    // If the video is paused we need to update the frame ourself (otherwise done in the video-thread),
    // but only if there is a video loaded.
    if (capture.isOpened() && is_paused()) {
        process_frame();
    }
}

/**
 * @brief video_player::set_contrast
 * Sets the contrast value (alpha value).
 * @param contrast Contrast parameter in range
 *                 CONTRAST_MIN to CONTRAST_MAX.
 */
void video_player::set_bright_cont(int b_value, double c_value) {
    frame_lock.lock();
    manipulator->set_brightness(b_value);
    manipulator->set_contrast(c_value);
    frame_lock.unlock();
    process_frame();
}

/**
 * @brief video_player::set_playback_speed
 * Sets the speed multiplyer for playback.
 * High value gives slower playback and vice versa.
 * @param speed multiplier value
 */
void video_player::set_speed_multiplier(double mult) {
    this->speed_multiplier = mult;
}

/**
 * @brief video_player::get_speed_multiplier
 * @return speed multiplier
 */
double video_player::get_speed_multiplier() {
    return this->speed_multiplier;
}

/**
 * @brief sets the speed multiplier for playback
 *        a positive int will increase the speed whilst a negative will decrease it
 * @param speed_steps
 */
void video_player::set_playback_speed(int speed_steps) {
    if (speed_steps > 0) {
        set_speed_multiplier(1.0 / (speed_steps * 2));
    } else if (speed_steps < 0) {
        set_speed_multiplier(std::abs(speed_steps) * 2);
    } else {
        set_speed_multiplier(1);
    }
}

/**
 * @brief video_player::set_overlay_tool
 * Sets the overlay tool's shape.
 * @param shape
 */
void video_player::set_overlay_tool(SHAPES shape) {
    video_overlay->set_tool(shape);
}

/**
 * @brief video_player::set_overlay_colour
 * Sets the overlay tool's colour.
 * @param colour
 */
void video_player::set_overlay_colour(QColor colour) {
    video_overlay->set_colour(colour);
}

/**
 * @brief video_player::undo_overlay
 * Undo the drawings on the overlay, if the overlay is visible
 * and the video is loaded and paused.
 */
void video_player::undo_overlay() {
    if (capture.isOpened()) {
        if (choosing_analysis_area) {
            analysis_area->undo();
        } else if (is_paused()) {
            video_overlay->undo(get_current_frame_num());
        }
        update_overlay();
    }
}

/**
 * @brief video_player::clear_overlay
 * Clear the drawings on the overlay, if the overlay is visible
 * and the video is loaded and paused.
 */
void video_player::clear_overlay() {
    if (capture.isOpened()) {
        if (choosing_analysis_area) {
            analysis_area->clear();
        } else if (is_paused()) {
            video_overlay->clear(get_current_frame_num());
        }
        update_overlay();
    }
}

/**
 * @brief video_player::toggle_analysis_area
 * Toggles the choosing of an analysis area.
 */
void video_player::toggle_analysis_area() {
    choosing_analysis_area = !choosing_analysis_area;
    update_overlay();
}

/**
 * @brief video_player::invert_analysis_area
 * Switches between choosing area for analysing and area for not analysing.
 */
void video_player::invert_analysis_area() {
    analysis_area->invert_area();
    update_overlay();
}

/**
 * @brief video_player::is_including_area
 * @return Returns true if the area should be included in the
 *         analysis, false if it should be excluded.
 */
bool video_player::is_including_area() {
    return analysis_area->is_including_area();
}

/**
 * @brief video_player::rotate_right
 * Rotates the video to the right by 90 degrees.
 */
void video_player::rotate_right() {
    // Rotating right means adding 1 and
    // starting over if larger than maximum,
    frame_lock.lock();
    rotate_direction = (rotate_direction + 1) % ROTATE_NUM;
    zoomer->flip();
    frame_lock.unlock();
    process_frame();
}

/**
 * @brief video_player::rotate_left
 * Rotates the video to the left by 90 degrees.
 */
void video_player::rotate_left() {
    // Rotating left means subtracting 1 and
    // starting over if larger than maximum.
    // Modulo handles positive values, so
    // minus 1 is the same as adding maximum-1.
    frame_lock.lock();
    rotate_direction = (rotate_direction + (ROTATE_NUM - 1)) % ROTATE_NUM;
    zoomer->flip();
    frame_lock.unlock();
    process_frame();
}

void video_player::video_mouse_pressed(QPoint pos) {
    if (capture.isOpened()) {
        if (is_paused()) {
            video_overlay->mouse_pressed(pos, get_current_frame_num());
        }
    } update_overlay();
}

void video_player::video_mouse_released(QPoint pos) {
    if (capture.isOpened()) {
        if (is_paused()) {
            video_overlay->mouse_released(pos, get_current_frame_num());
        }
    } update_overlay();
}
void video_player::video_mouse_moved(QPoint pos) {
    if (capture.isOpened()) {
        if (is_paused()) {
            video_overlay->mouse_moved(pos, get_current_frame_num());
        }
    } update_overlay();
}

/**
 * @brief video_player::scale_position
 * Recalculates the given mouse position from a position in the window
 * the video is shown in to the position on the video frame.
 * @param pos The position to be recalculated.

void video_player::scale_position(QPoint &pos) {
    int video_frame_width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    int video_frame_height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);

    // Calculate rotated coordinates from the coordinates
    // on the QLabel where the frame is shown. The frame is
    // centered vertically, so the empty part of the QLabel
    // at the top needs to be subtracted.
    int rotated_x = 0;
    int rotated_y = 0;
    if (rotate_direction == ROTATE_90) {
        rotated_x = (pos.y() - (double) (video_frame->height() - frame_width) / 2);
        rotated_y = frame_height - pos.x();
    } else if (rotate_direction == ROTATE_180) {
        rotated_x = frame_width - pos.x();
        rotated_y = ((video_frame->height() - pos.y()) - (double) (video_frame->height() - frame_height) / 2);
    } else if (rotate_direction == ROTATE_270) {
        rotated_x = ((video_frame->height() - pos.y()) - (double) (video_frame->height() - frame_width) / 2);
        rotated_y = pos.x();
    } else if (rotate_direction == ROTATE_NONE) {
        rotated_x = pos.x();
        rotated_y = (pos.y() - (double) (video_frame->height() - frame_height) / 2);
    }

    // Calculate the scale ratio between the actual video
    // size and the size of the (scaled) video frame shown in the gui.
    double x_scale_ratio = (double) video_frame_width/frame_width;
    double y_scale_ratio = (double) video_frame_height/frame_height;

    // Calculate the coordinates on the original-sized frame,
    // by multiplying with the ratio.
    double x_scale = x_scale_ratio * rotated_x;
    double y_scale = y_scale_ratio * rotated_y;

    // Calculate the coordinates on the actual video from
    // the coordinates on the zoomed frame.
    double x_zoom_ratio = (double) zoom_area->get_width()/video_frame_width;
    double y_zoom_ratio = (double) zoom_area->get_height()/video_frame_height;
    double x_video = zoom_area->get_x() + (double) x_zoom_ratio * x_scale;
    double y_video = zoom_area->get_y() + (double) y_zoom_ratio * y_scale;

    pos.setX(x_video);
    pos.setY(y_video);
}
*/

/**
 * @brief video_player::video_open
 * @return a bool that is true as long as a video is open.
 */
bool video_player::video_open() {
    return capture.isOpened();
}

/**
 * @brief video_player::get_video_width
 * @return original width of the video
 */
int video_player::get_video_width() {
    return original_width;
}

/**
 * @brief video_player::get_video_height
 * @return original height of the video
 */
int video_player::get_video_height() {
    return original_height;
}

/**
 * @brief video_player::get_analysis_area_polygon
 * @return analysis area polygon
 */
std::vector<cv::Point>* video_player::get_analysis_area_polygon() {
    return analysis_area->get_polygon();
}
