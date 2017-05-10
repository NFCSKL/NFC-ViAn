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
video_player::video_player(QMutex* mutex, QWaitCondition* paused_wait, QLabel* label, QObject* parent) : QThread(parent) {
    m_mutex = mutex;
    m_paused_wait = paused_wait;
    video_frame = label;
    QRect rec = QApplication::desktop()->screenGeometry();
    screen_height = rec.height();
    screen_width = rec.width();
}

/**
 * @brief video_player::~video_player
 */
video_player::~video_player() {
    delete video_overlay;
    delete zoom_area;
    delete analysis_area;
    capture.release();
}

/**
 * @brief video_player::load_video
 * This method loads a video from file.
 * @param filename
 * @return whether video is loaded
 */
bool video_player::load_video(string filename) {
    if (capture.isOpened())
        capture.release();

    capture.open(filename);

    if (capture.isOpened()) {
        frame_rate = capture.get(CV_CAP_PROP_FPS);
        num_frames = capture.get(CAP_PROP_FRAME_COUNT);
        file_path = filename;
        video_paused = false;
        zoom_area->set_size(capture.get(CAP_PROP_FRAME_WIDTH), capture.get(CAP_PROP_FRAME_HEIGHT));
        start();
        return true;
    } else {
        return false;
    }
}

/**
 * @brief video_player::run
 * This function is called whenever the thread is started or put out of sleep.
 * It houses the main loop for fetching frames from the currently played
 * video file and sending them to the GUI.
 */
void video_player::run()  {
    video_stopped = false;
    video_paused = false;
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
}

/**
 * @brief show_frame
 * Calculates and emits the current frame to GUI.
 */
void video_player::show_frame() {
    emit update_current_frame(capture.get(CV_CAP_PROP_POS_FRAMES));
    emit processed_image(img);
}

/**
 * @brief video_player::convert_frame
 * Converts the current frame to a QImage,
 * including the zoom and overlay.
 * @param scale Bool indicating if the frame should be scaled or not.
 */
void video_player::convert_frame(bool scale) {
    if (frame.cols == 0 || frame.rows == 0) {
        // Do nothing
        return;
    }
    cv::Mat processed_frame;

    // Process frame (draw overlay, zoom, scaling, contrast/brightness, rotation)
    processed_frame = process_frame(frame, scale);

    if (processed_frame.channels() == 3) {
        cv::Mat RGBframe;
        cvtColor(processed_frame, RGBframe,CV_BGR2RGB);
        img = QImage((const uchar *) RGBframe.data, RGBframe.cols,
                  RGBframe.rows, RGBframe.step, QImage::Format_RGB888);
        img.bits(); // enforce deep copy
    } else {
        img = QImage((const unsigned char*)(processed_frame.data),
                             processed_frame.cols,processed_frame.rows,QImage::Format_Indexed8);
        img.bits(); // enforce deep copy
    }
}

/**
 * @brief video_player::process_frame
 * Draws overlay, zooms, scales, changes contrast/brightness on the frame.
 * @param frame Frame to draw on.
 * @param scale Bool indicating if the frame should be scaled or not.
 * @return Returns the processed frame.
 */
cv::Mat video_player::process_frame(cv::Mat &src, bool scale) {
    // Copy the frame, so that we don't alter the original frame (which will be reused next draw loop).
    cv::Mat processed_frame = src.clone();
    processed_frame = analysis_overlay->draw_overlay(processed_frame, get_current_frame_num());
    processed_frame = video_overlay->draw_overlay(processed_frame, get_current_frame_num());

    if (choosing_zoom_area) {
        processed_frame = zoom_area->draw(processed_frame);
    }
    if (choosing_analysis_area) {
        processed_frame = analysis_area->draw(processed_frame);
    }

    processed_frame = zoom_frame(processed_frame);
    processed_frame = contrast_frame(processed_frame);

    //Check if the dimensions of the frame are reasonable
    bool frame_dimensions_limited = frame_width > 0 && frame_height > 0 &&
            frame_width <= screen_width && frame_height <= screen_height;

    //Check if video is shown in original dimensions, which means no scaling is needed
    bool original_dimensions_shown = frame_width == capture.get(CV_CAP_PROP_FRAME_WIDTH) &&
            frame_height == capture.get(CV_CAP_PROP_FRAME_HEIGHT);

    //Scales the frame if these criteria are met
    if (scale && frame_dimensions_limited && !original_dimensions_shown) {
        processed_frame = scale_frame(processed_frame);
    }

    // Rotates the frame, according to the choosen direction.
    // If direction is in the valid range the frame is rotated.
    if (ROTATE_MIN <= rotate_direction && rotate_direction <= ROTATE_MAX) {
        cv::rotate(processed_frame, processed_frame, rotate_direction);
    }

    return processed_frame;
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
 * @brief video_overlay::zoom_frame
 * Zooms in the frame, with the choosen zoom level.
 * @param frame Frame to zoom in on on.
 * @return Returns the zoomed frame.
 */
cv::Mat video_player::zoom_frame(cv::Mat &src) {
    // The area to zoom in on.
    cv::Rect roi = zoom_area->get_zoom_area();
    cv::Mat zoomed_frame;
    // Crop out the area and resize to video size.
    resize(src(roi), zoomed_frame, src.size());
    return zoomed_frame;
}

/**
 * @brief video_player::contrast_frame
 * Adds contrast and brightness to the frame.
 * @param frame Frame to manipulate.
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
    video_paused = false;
    video_stopped = false;
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
    set_current_frame_num(0);
    convert_frame(true);
    show_frame();
}

/**
 * @brief video_player::update_frame
 * @param frame_nbr
 * Updates the current frame if frame_nbr is valid.
 */
void video_player::update_frame(int frame_nbr) {
    if (set_current_frame_num(frame_nbr)) {
        convert_frame(true);
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
        convert_frame(true);
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
        convert_frame(true);
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
        convert_frame(true);
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
        convert_frame(true);
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
 * @brief video_player::dec_playback_speed
 * Decreases the playback speed by a factor of 2
 */
void video_player::dec_playback_speed() {
    if (this->speed_multiplier < MAX_SPEED_MULT) {
        this->set_speed_multiplier(this->get_speed_multiplier()*SPEED_STEP_MULT);
    }
}

/**
 * @brief video_player::inc_playback_speed
 * Increases the playback speed by a factor of 2
 */
void video_player::inc_playback_speed() {
    if (this->speed_multiplier > MIN_SPEED_MULT) {
        this->set_speed_multiplier(this->get_speed_multiplier()/SPEED_STEP_MULT);
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
 * @brief video_player::zoom_in
 * Sets a state in the video overlay
 * for the user to choose an area,
 * if there is a video loaded.
 */
void video_player::zoom_in() {
    if (capture.isOpened()) {
        choosing_zoom_area = true;
        zoom_area->reset_pos();
        QApplication::setOverrideCursor(Qt::CrossCursor); // Set zoom cursor.
    }
}

/**
 * @brief video_player::zoom_out
 * Resets zoom level to the full video size,
 * if there is a video loaded.
 * Also updates the frame shown in the GUI.
 */
void video_player::zoom_out() {
    if (capture.isOpened()) {
        zoom_area->reset_zoom_area();
        update_overlay();
    }
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
 * @brief video_player::video_mouse_pressed
 * If the user is choosing a zoom area and there
 * is a video loaded, its position is updated.
 * Otherwise starts drawing on the overlay, if
 * the overlay is visible and the video is loaded
 * and paused.
 * The frame in the GUI is updated.
 * @param pos Mouse coordinates.
 */
void video_player::video_mouse_pressed(QPoint pos) {
    if (capture.isOpened()) {
        scale_position(pos);
        if (choosing_zoom_area) {
            zoom_area->set_start_pos(pos);
            zoom_area->update_drawing_pos(pos);
        } else if (choosing_analysis_area) {
            // When choosing analysis area, a point is choosen when mouse released.
            return;
        } else if (is_paused()) {
            video_overlay->mouse_pressed(pos, get_current_frame_num());
        }
        update_overlay();
    }
}

/**
 * @brief video_player::video_mouse_released
 * If the user is choosing a zoom area is choosen.
 * otherwise ends drawing on the overlay, if the overlay is visible
 * and the video is loaded and paused.
 * If the video is paused, the frame in the GUI is updated.
 * @param pos Mouse coordinates.
 */
void video_player::video_mouse_released(QPoint pos) {
    if (capture.isOpened()) {
        scale_position(pos);
        if (choosing_zoom_area) {
            zoom_area->update_drawing_pos(pos);
            zoom_area->choose_area();
            choosing_zoom_area = false;
            QApplication::setOverrideCursor(Qt::ArrowCursor); // Restore cursor.
        } else if (choosing_analysis_area) {
            analysis_area->add_point(pos);
        } else if (is_paused()) {
            video_overlay->mouse_released(pos, get_current_frame_num());
        }
        update_overlay();
    }
}

/**
 * @brief video_player::video_mouse_moved
 * If the user is choosing a zoom area and there
 * is a video loaded, its position is updated.
 * Otherwise updates drawing on the overlay, if the overlay is visible
 * and the video is loaded and paused.
 * If the video is paused, the frame in the GUI is updated.
 * @param pos Mouse coordinates.
 */
void video_player::video_mouse_moved(QPoint pos) {
    if (capture.isOpened()) {
        scale_position(pos);
        if (choosing_zoom_area) {
            zoom_area->update_drawing_pos(pos);
        } else if (choosing_analysis_area) {
            // When choosing analysis area, a point is choosen when mouse released.
            return;
        } else if (is_paused()) {
            video_overlay->mouse_moved(pos, get_current_frame_num());
        }
        update_overlay();
    }
}

/**
 * @brief video_player::scale_position
 * Recalculates the given mouse position from a position in the window
 * the video is shown in to the position on the video frame.
 * @param pos The position to be recalculated.
 */
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

/**
 * @brief video_player::get_current_frame_unscaled
 * Creates, converts, processes the current frame and returns it.
 */
QImage video_player::get_current_frame_unscaled() {
    convert_frame(false);

    return img;
}

/**
 * @brief video_player::scaling_event
 * This slot gets called when the video QLabel gets resized.
 * Sets the video width and height to match.
 * @param width
 * @param height
 */
void video_player::scaling_event(int new_width, int new_height) {
    if (!capture.isOpened()) {
        return;
    }

    int video_width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    int video_height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    float height_ratio = float(new_height)/float(video_height);
    float width_ratio = float(new_width)/float(video_width);


    //This statement ensures that the original aspect ratio of the video is kept when scaling
    if (width_ratio >= height_ratio) {
        frame_width = int(video_width * height_ratio);
        frame_height = new_height;
    } else {
        frame_width = new_width;
        frame_height = int(video_height * width_ratio);
    }

    update_overlay();
}

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
