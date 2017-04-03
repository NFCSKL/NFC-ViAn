#include "video_player.h"
#include <string>
#include <iostream>
#include "GUI/mainwindow.h"
#include <thread>
#include <QWaitCondition>
#include <qpainter.h>
#include <qdebug.h>


using namespace std;
using namespace cv;

/**
 * @brief video_player::video_player
 * @param parent
 */
video_player::video_player(QMutex* mutex, QWaitCondition* paused_wait, QObject* parent) : QThread(parent) {
    video_overlay = new Overlay();
    m_mutex = mutex;
    m_paused_wait = paused_wait;
}

/**
 * @brief video_player::~video_player
 */
video_player::~video_player() {
    delete video_overlay;
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
    capture.open(filename);

    if (capture.isOpened()) {
        frame_rate = capture.get(CV_CAP_PROP_FPS);
        num_frames = capture.get(CAP_PROP_FRAME_COUNT);
        video_paused = false;
        zoom_area->set_size(capture.get(CAP_PROP_FRAME_WIDTH), capture.get(CAP_PROP_FRAME_HEIGHT));
        start();
        return true;
    }
    else
        cout << "Could not load file \"" << filename << "\"" << endl;
        return false;
}

/**
 * @brief video_player::run
 * This function is called whenever the thread is started or put out of sleep.
 * It houses the main loop for fetching frames from the currently played
 * video file and sending them to the GUI.
 */
void video_player::run()  {
    qDebug() << "Starting video thread";
    video_stopped = false;
    video_paused = false;
    int delay = (1000/frame_rate);
    set_current_frame_num(0);
    while (!video_stopped && capture.read(frame)) {
        const clock_t begin_time = std::clock();
        convert_frame();

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
            qDebug() << "Video paused. Sleeping thread";
            m_paused_wait->wait(m_mutex);
            video_paused = false;
        }
        m_mutex->unlock();
    }
    video_stopped = true;
    capture.set(CV_CAP_PROP_POS_FRAMES, 0);
    emit update_current_frame(0);
    qDebug() << "Terminating video thread";
}

/**
 * @brief update_frame
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
 */
void video_player::convert_frame() {
    qDebug() << "Convert frame";
    if (frame.cols == 0 || frame.rows == 0) {
        // TODO
        // When reaching the last frame the mat object will sometimes
        // have cols and rows set to zero and then it cannot be coverted
        // As a result the program crashes. This needs to be fixed
        qDebug() << "Error with current frame";
        qDebug() << frame.cols << " " << frame.rows;
        return;
    }
    cv::Mat processed_frame;

    // Process frame (draw overlay, zoom, scaling, contrast/brightness)
    processed_frame = process_frame(frame);


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
 * @return Returns the processed frame.
 */
cv::Mat video_player::process_frame(cv::Mat &frame) {
    // Copy the frame, so that we don't alter the original frame (which will be reused next draw loop).
    qDebug() << "Process frame" << frame.rows << " " << frame.cols;
    cv::Mat processed_frame = frame.clone();
    qDebug() << "Cloned";
    processed_frame = video_overlay->draw_overlay(processed_frame, get_current_frame_num());
    if (choosing_zoom_area) {
        processed_frame = zoom_area->draw(processed_frame);
    }
    if (choosing_analysis_area) {
        processed_frame = analysis_area->draw(processed_frame);
    }
    processed_frame = zoom_frame(processed_frame);
    processed_frame = contrast_frame(processed_frame);

    cv::Mat scaled_frame;
    if (frame_width != capture.get(CV_CAP_PROP_FRAME_WIDTH) || frame_height != capture.get(CV_CAP_PROP_FRAME_HEIGHT)) {
        scaled_frame = scale_frame(processed_frame);
    } else {
        scaled_frame = processed_frame;
    }
    return scaled_frame;
}

/**
 * @brief video_player::scale_frame
 * Scales the video frame to match the resolution of the video window.
 * @param src
 * @return
 */
cv::Mat video_player::scale_frame(cv::Mat &src) {
    qDebug() << frame_width << " " << frame_height;
    cv::Size size;
    if (frame_width <= 0 || frame_height <= 0) {
        // TODO
        // Both frame_width and frame_height are unsigned ints and thus never < 0
        // Need to check for bigger sizes
        size = cv::Size(capture.get(CV_CAP_PROP_FRAME_WIDTH),capture.get(CV_CAP_PROP_FRAME_HEIGHT));
        frame_width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
        frame_height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
        qDebug() << frame_width << " " << frame_height;
    } else {
        size = cv::Size(frame_width,frame_height);
    }

    cv::Mat dst;

    cv::resize(src,dst,size); //resize image
    return dst;
}

/**
 * @brief video_overlay::zoom_frame
 * Zooms in the frame, with the choosen zoom level.
 * @param frame Frame to zoom in on on.
 * @return Returns the zoomed frame.
 */
cv::Mat video_player::zoom_frame(cv::Mat &frame) {
    // The area to zoom in on.
    cv::Rect roi = zoom_area->get_zoom_area();
    cv::Mat zoomed_frame;
    // Crop out the area and resize to video size.
    resize(frame(roi), zoomed_frame, frame.size());
    return zoomed_frame;
}

/**
 * @brief video_player::contrast_frame
 * Adds contrast and brightness to the frame.
 * @param frame Frame to manipulate.
 * @return Returns the manipulated frame.
 */
cv::Mat video_player::contrast_frame(cv::Mat &frame) {
    // Create image for the modified frame.
    Mat modified_frame;
    // Do the operation modified_frame = alpha * frame + beta
    frame.convertTo(modified_frame, -1, alpha, beta);
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
            new_frame_num = frame_num - 1;
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
    convert_frame();
    show_frame();
}

/**
 * @brief video_player::update_frame
 * @param frame_nbr
 * Updates the current frame if frame_nbr is valid.
 */
void video_player::update_frame(int frame_nbr) {
    if (set_current_frame_num(frame_nbr)) {
        convert_frame();
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
        convert_frame();
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
        convert_frame();
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
        convert_frame();
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
        convert_frame();
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
    if (capture.isOpened() && is_paused()) {
        video_overlay->undo(get_current_frame_num());
        update_overlay();
    }
}

/**
 * @brief video_player::clear_overlay
 * Clear the drawings on the overlay, if the overlay is visible
 * and the video is loaded and paused.
 */
void video_player::clear_overlay() {
    if (capture.isOpened() && is_paused()) {
        video_overlay->clear(get_current_frame_num());
        update_overlay();
    }
}

/**
 * @brief video_player::toggle_analysis_area
 * Toggles the choosing of an analysis area.
 */
void video_player::toggle_analysis_area() {
    if (capture.isOpened()) {
        choosing_analysis_area = !choosing_analysis_area;
        update_overlay();
    }
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

    // Calculate the scale ratio between the actual video
    // size and the size of the frame shown in the gui.
    double x_scale_ratio = (double) video_frame_width/frame_width;
    double y_scale_ratio = (double) video_frame_height/frame_height;

    // Calculate the coordinates on the original-sized frame,
    // from the coordinates on the QLabel where the frame is shown.
    // (Multiply with the ratio, and subtract the empty parts of the QLabel,
    // only subtracting the top and left side, hence division by 2.)
    double x_scale = x_scale_ratio * pos.x() - (qlabel_width - frame_width) / 2;
    double y_scale = y_scale_ratio * pos.y() - (qlabel_height - frame_height) / 2;

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
 * @brief video_player::export_current_frame
 * Stores the current frame in the specified folder.
 * The stored frame will have the sam resolution as the video.
 * @param filename Path to the folder to store the file in.
 */
void video_player::export_current_frame(QString path_to_folder) {
    convert_frame();

    // Add "/FRAME_NR.tiff" to the path.
    path_to_folder.append("/");
    path_to_folder.append(QString::number(get_current_frame_num()));
    path_to_folder.append(".tiff");

    QImageWriter writer(path_to_folder, "tiff");
    writer.write(img);
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

    qlabel_width = new_width;
    qlabel_height = new_height;

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
}

/**
 * @brief video_player::video_open
 * @return a bool that is true as long as a video is open.
 */
bool video_player::video_open() {
    return capture.isOpened();
}
