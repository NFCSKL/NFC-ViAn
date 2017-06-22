#include "video_player.h"
#include <string>
#include <iostream>
#include "GUI/mainwindow.h"
#include <thread>
#include <QWaitCondition>
#include <QDesktopWidget>
#include <qpainter.h>
#include <qdebug.h>


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
}

/**
 * @brief video_player::~video_player
 */
video_player::~video_player() {
    delete analysis_area;
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
        video_paused = false;
        emit frame_count(num_frames);
        emit total_time(int(num_frames / frame_rate));
        emit capture_frame_size(QSize(original_width, original_height));
        capture.set(CV_CAP_PROP_POS_FRAMES, 0);
        video_paused = true;
        video_stopped = false;
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
    choosen_interpol = cv::INTER_NEAREST;
    scale_factor = 1;
}

/**
 * @brief video_player::run
 * This function is called whenever the thread is started or put out of sleep.
 * It houses the main loop for fetching frames from the currently played
 * video file and sending them to the GUI.
 */
void video_player::run()  {
    int delay = (1000/frame_rate);
    set_current_frame_num(0);
    while (!video_stopped && capture.read(frame)) {
        const clock_t begin_time = std::clock();

        convert_frame(true);

        int conversion_time = int((std::clock()-begin_time)*1000.0 /CLOCKS_PER_SEC);
        if (delay - conversion_time > 0) {
            this->msleep(delay - conversion_time);
        }

        show_frame();

        if (set_new_frame) {
            // A new frame has been set outside the loop, change it
            capture.set(CV_CAP_PROP_POS_FRAMES, new_frame_num);
            set_new_frame = false;
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
    capture.set(CV_CAP_PROP_POS_FRAMES, 0);
    emit update_current_frame(0);
    video_paused = false;
    qDebug() << "end of thread loop";
}

/**
 * @brief show_frame
 * Calculates and emits the current frame to GUI.
 */
void video_player::show_frame() {
    emit update_current_frame(capture.get(CV_CAP_PROP_POS_FRAMES) - 1);
    emit processed_image(manipulated_frame);
}

/**
 * @brief video_player::convert_frame TODO SHOULD NOW BE DONE IN FRAMEWIDHGET
 * Converts the current frame to a QImage,
 * including the zoom and overlay.
 * @param scale Bool indicating if the frame should be scaled or not.
 */
void video_player::convert_frame(bool scale) {
    if (frame.cols == 0 || frame.rows == 0) {
        // Do nothing
        return;
    }

    // Process frame (draw overlay, zoom, scaling, contrast/brightness, rotation)
    process_frame(scale);
}

/**
 * @brief video_player::process_frame
 * Draws overlay, zooms, scales, changes contrast/brightness on the frame.
 * @param src Frame to draw on.
 * @param scale Bool indicating if the frame should be scaled or not.
 * @return Returns the processed frame.
 */
void video_player::process_frame(bool scale) {
    // Copy the frame, so that we don't alter the original frame (which will be reused next draw loop).
    manipulated_frame = frame.clone();
//    processed_frame = analysis_overlay->draw_overlay(processed_frame, get_current_frame_num());
//    processed_frame = video_overlay->draw_overlay(processed_frame, get_current_frame_num());

//    if (choosing_analysis_area) {
//        processed_frame = analysis_area->draw(processed_frame);
//    }

//    processed_frame = contrast_frame(processed_frame);
    if (scale_factor != 1) scale_mat(scale_factor);

//    //Scales the frame if these criteria are met
//    if (scale && frame_dimensions_limited && !original_dimensions_shown) {
//        processed_frame = scale_frame(processed_frame);
//    }

//    // Rotates the frame, according to the choosen direction.
//    // If direction is in the valid range the frame is rotated.
//    if (ROTATE_MIN <= rotate_direction && rotate_direction <= ROTATE_MAX) {
//        cv::rotate(processed_frame, processed_frame, rotate_direction);
//    }
}


/**
 * @brief video_player::scale_frame
 * Scales the video frame to match the resolution of the video window.
 * Before using this method, a check that frame_width and frame_height
 * have reasonable values is needed.
 * @param src
 * @return
 */
cv::Mat video_player::scale_frame(cv::Mat &src) {
    cv::Size size(frame_width,frame_height);
    cv::Mat dst(size,src.type());
    cv::resize(src,dst,size); //resize frame
    return dst;
}

/**
 * @brief video_player::zoom_frame
 * Zooms in the frame, with the choosen zoom level.
 * @param src Frame to zoom in on on.
 * @return Returns the zoomed frame.
 */
void video_player::update_zoom(double zoom_factor) {
    qDebug() << zoom_factor;
    scale_factor *= zoom_factor;
    process_frame(zoom_factor);
    emit processed_image(manipulated_frame);
}

void video_player::set_zoom(double zoom_factor) {
    scale_factor = zoom_factor;
    process_frame(zoom_factor);
    emit processed_image(manipulated_frame);
}

void video_player::scale_mat(double scale) {
    int interpol_method = 0;
    if (scale_factor > 1) {
        interpol_method = choosen_interpol;
    } else if (scale_factor < 1) {
        interpol_method = cv::INTER_AREA;
    }
    cv::Mat scaled_frame;
    cv::resize(manipulated_frame, manipulated_frame, cv::Size(), scale, scale, interpol_method);
}

/**
 * @brief video_player::contrast_frame
 * Adds contrast and brightness to the frame.
 * @param src Frame to manipulate.
 * @return Returns the manipulated frame.
 */
cv::Mat video_player::contrast_frame(cv::Mat &src) {
    // Create image for the modified frame.
    Mat modified_frame;
    // Do the operation modified_frame = alpha * frame + beta
    src.convertTo(modified_frame, -1, alpha, beta);
    return modified_frame;
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
 * @brief video_player::is_showing_analysis_overlay
 * @return Returns true if the analysis overlay is showing, else false.
 */
bool video_player::is_showing_analysis_overlay() {
    return analysis_overlay->is_showing_overlay();
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
 * @brief video_player::get_file_name
 * @return Returns the file name of the video that has
 *         been loaded into the video player.
 */
std::string video_player::get_file_name() {
    std::size_t found = file_path.find_last_of("/");
    return file_path.substr(found+1);
}

/**
 * @brief video_player::set_current_frame_num
 * Sets the current frame to the specified number, if it's within the video.
 * @param frame_nbr The number to set the currently read frame to (0-based index).
 * @return Return true if successful, false if the specified number is outside the video.
 */
bool video_player::set_current_frame_num(int frame_nbr) {
    if (frame_nbr >= 0 && frame_nbr < get_num_frames()) {
        // capture.set() sets the number of the frame to be read.
        if (video_paused) {
            capture.set(CV_CAP_PROP_POS_FRAMES, frame_nbr);
            capture.read(frame);
        } else {
            set_new_frame = true;
            new_frame_num = frame_nbr;
        }
        return true;
    }
    return false;
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
 * @brief video_player::on_set_playback_frame
 * Updates the frame directly if the video is paused.
 * Otherwise it saves the frame number which later on
 * updates in the run function
 * @param frame_num
 */
void video_player::on_set_playback_frame(int frame_num) {
    if (video_paused) {
        update_frame(frame_num - 1);
    } else {
        if (frame_num >= 0 && frame_num < get_num_frames()) {
            set_new_frame = true;
            new_frame_num = frame_num;
        } else {
            set_new_frame = false;
        }
    }
}

/**
 * @brief video_player::next_frame
 * Moves the playback one frame forward.
 */
void video_player::next_frame() {
    update_frame(get_current_frame_num() + 1);
}

/**
 * @brief video_player::previous_frame
 * Moves the playback one frame backward.
 */
void video_player::previous_frame() {
    update_frame(get_current_frame_num() - 1);
}

/**
 * @brief on_play_video
 * Slot function to be used from the GUI thread
 * Sets the paused and stopped bools to false
 */
void video_player::on_play_video() {
    qDebug() << QThread::currentThreadId();
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
    qDebug() << QThread::currentThreadId();
    video_paused = true;
}

/**
 * @brief on_stop_video
 * Slot function to be used from the GUI thread
 * Sets the stopped bool to true and the paused bool to false.
 * Sets playback frame to the start of the video and updates the GUI.
 */
void video_player::on_stop_video() {
    qDebug() << "stopping video";
    video_stopped = true;
    video_paused = false;
    set_current_frame_num(0);
    //convert_frame(true);
    show_frame();
}

/**
 * @brief video_player::update_frame
 * @param frame_nbr
 * Updates the current frame if frame_nbr is valid.
 */
void video_player::update_frame(int frame_nbr) {
    if (set_current_frame_num(frame_nbr)) {
        //convert_frame(true);
        show_frame();
    }
}

/**
 * @brief video_player::update_overlay
 * Updates the current frame if the video is loaded and paused.
 */
void video_player::update_overlay() {
    // If the video is paused we need to update the frame ourself (otherwise done in the video-thread),
    // but only if there is a video loaded.
    if (capture.isOpened() && is_paused()) {
        //convert_frame(true);
        show_frame();
    }
}

/**
 * @brief video_player::set_slider_frame
 * @param frame_nbr
 * This method is called when the slider is moved and is used to call the private method
 * update_frame with the desired frame number.
 */
void video_player::set_slider_frame(int frame_nbr) {
    update_frame(frame_nbr);
}


/** @brief video_player::reset_brightness_contrast
 * Resets contrast and brightness to default values.
 */
void video_player::reset_brightness_contrast() {
    alpha = CONTRAST_DEFAULT;
    beta = BRIGHTNESS_DEFAULT;
    if (capture.isOpened()) {
        //convert_frame(true);
        show_frame();
    }
}

/**
 * @brief video_player::set_contrast
 * Sets the contrast value (alpha value).
 * @param contrast Contrast parameter in range
 *                 CONTRAST_MIN to CONTRAST_MAX.
 */
void video_player::set_contrast(double contrast) {
    alpha = std::min(CONTRAST_MAX, std::max(CONTRAST_MIN, contrast));
    if (capture.isOpened()) {
        //convert_frame(true);
        show_frame();
    }
}

/**
 * @brief video_player::set_brightness
 * Sets the brightness value (beta value).
 * @param brightness Brightness parameter in range
 *                   BRIGHTNESS_MIN to BRIGHTNESS_MAX.
 */
void video_player::set_brightness(int brightness) {
    beta = std::min(BRIGHTNESS_MAX, std::max(BRIGHTNESS_MIN, brightness));
    if (capture.isOpened()) {
        //convert_frame(true);
        show_frame();
    }
}

/**
 * @brief video_player::get_contrast
 * @return Returns contrast parameter in range 0 to 255.
 */
double video_player::get_contrast() {
    return alpha;
}

/**
 * @brief video_player::get_brightness
 * @return Returns brightness parameter in range 0 to 255.
 */
int video_player::get_brightness() {
    return beta;
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
 * @brief video_player::toggle_overlay
 * Toggles the showing of the overlay, and if video is paused updates
 * the frame in the GUI to show with/without the overlay.
 */
void video_player::toggle_overlay() {
    video_overlay->toggle_overlay();
    update_overlay();
}

/**
 * @brief video_player::toggle_analysis_overlay
 * Toggles the showing of the analysis overlay, and if video is paused updates
 * the frame in the GUI to show with/without the overlay.
 */
void video_player::toggle_analysis_overlay() {
    analysis_overlay->toggle_showing();
    update_overlay();
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
    if (capture.isOpened()) {
        // Rotaing right means adding 1 and
        // starting over if larger than maximum,
        rotate_direction = (rotate_direction + 1) % ROTATE_NUM;
        update_overlay();
    }
}

/**
 * @brief video_player::rotate_left
 * Rotates the video to the left by 90 degrees.
 */
void video_player::rotate_left() {
    if (capture.isOpened()) {
        // Rotaing left means subtracting 1 and
        // starting over if larger than maximum.
        // Modulo handles positive values, so
        // minus 1 is the same as adding maximum-1.
        rotate_direction = (rotate_direction + (ROTATE_NUM - 1)) % ROTATE_NUM;
        update_overlay();
    }
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
    return capture.get(CV_CAP_PROP_FRAME_WIDTH);
}

/**
 * @brief video_player::get_video_height
 * @return original height of the video
 */
int video_player::get_video_height() {
    return capture.get(CV_CAP_PROP_FRAME_HEIGHT);
}

/**
 * @brief video_player::on_set_analysis_results
 * Sets analysis results to be used to draw results on video.
 * @param analysis results
 */
void video_player::on_set_analysis_results(Analysis analysis) {
    for (int frame_num = 0; frame_num < get_num_frames(); ++frame_num) {
        for (cv::Rect rect : analysis.get_detections_on_frame(frame_num)) {
            analysis_overlay->add_area(frame_num,rect);
        }
    }
}

/**
 * @brief video_player::get_analysis_area_polygon
 * @return analysis area polygon
 */
std::vector<cv::Point>* video_player::get_analysis_area_polygon() {
    return analysis_area->get_polygon();
}

/**
 * @brief video_player::clear_analysis_overlay
 */
void video_player::clear_analysis_overlay(){
    delete analysis_overlay;
    analysis_overlay = new AnalysisOverlay();
}

