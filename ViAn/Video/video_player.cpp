#include "video_player.h"
#include <string>
#include <iostream>
#include "GUI/mainwindow.h"
#include <thread>
#include <QWaitCondition>
#include <qpainter.h>


using namespace std;
using namespace cv;

/**
 * @brief video_player::video_player
 * @param parent
 */
video_player::video_player(QObject* parent) : QThread(parent) {
    video_overlay = new overlay();
}

/**
 * @brief video_player::~video_player
 */
video_player::~video_player() {
    delete video_overlay;
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
 * @brief video_player::play
 * Toggles the video_paused boolean
 */
void video_player::play_pause() {
    video_paused = !video_paused;
}

/**
 * @brief video_player::stop_video
 * Sets stop related bools to their correct values and sets the current playback frame to be 0.
 */
void video_player::stop_video() {
    stop = true;
    set_playback_frame(0);
    if (video_paused) {
        video_paused = false;
    }
}

/**
 * @brief video_player::run
 * This function is called whenever the thread is started or put out of sleep.
 * It houses the main loop for fetching frames from the currently played
 * video file and sending them to the GUI.
 */
void video_player::run()  {
    stop = false;
    video_paused = false;
    int delay = (1000/frame_rate);
    capture.set(CV_CAP_PROP_POS_FRAMES,current_frame);
    while(!stop && !video_paused && capture.read(frame)){
        show_frame();

        this->msleep(delay);

    }
    //Saves the current frame of the video if the video is paused.
    if (video_paused) {
        current_frame = capture.get(CV_CAP_PROP_POS_FRAMES);
    } else if (stop) {
        current_frame = 0;
    }
}

/**
 * @brief update_frame
 * Calculates and emits the current frame to GUI.
 */
void video_player::show_frame() {
    emit currentFrame(capture.get(CV_CAP_PROP_POS_FRAMES));
    convert_frame();
    emit processedImage(img);
}

/**
 * @brief video_player::convert_frame
 * Converts the current frame, including the overlay, to a QImage.
 * @param frame The current frame in the video
 */
void video_player::convert_frame() {
    cv::Mat zoomed_frame;
    zoomed_frame = zoom_frame(frame);

    if (zoomed_frame.channels() == 3) {
        cv::cvtColor(zoomed_frame, RGBframe, CV_BGR2RGB);
        img = QImage((const unsigned char*)(RGBframe.data),
                          RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
    } else {
        img = QImage((const unsigned char*)(zoomed_frame.data),
                             zoomed_frame.cols,zoomed_frame.rows,QImage::Format_Indexed8);
    }
    video_overlay->draw_overlay(img, capture.get(CV_CAP_PROP_POS_FRAMES));
    if (choosing_zoom_area) {
        zoom_area->draw(img);
    }
}

/**
 * @brief video_overlay::draw_overlay
 * Zooms in the frame, with the choosen zoom level.
 * @param frame Frame to draw on.
 * @return Returns the frame including the zoom and overlay.
 */
cv::Mat video_overlay::zoom_frame(cv::Mat &frame) {
    cv::Rect roi = zoom_area->get_zoom_area();

    cv::Mat zoomed_frame;
    resize(frame(roi), zoomed_frame, frame.size());
    return zoomed_frame;
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
    return stop;
}

/**
 * @brief video_player::is_showing_overlay
 * Returns true if the overlay tool is showing, else false.
 * @return
 */
bool video_player::is_showing_overlay() {
    return video_overlay->is_showing_overlay();
}

/**
 * @brief video_player::get_num_frames
 * @return number of frames in video file
 */
int video_player::get_num_frames() {
    return num_frames;
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
 * @brief video_player::set_playback_frame
 * Moves the playback to the frame specified by frame_num
 * @param frame_num
 */
bool video_player::set_playback_frame(int frame_num) {

    if (frame_num < get_num_frames() && frame_num >= 0) {
        current_frame = frame_num;
        return true;
    }
    return false;
}

/**
 * @brief video_player::next_frame
 * Moves the playback one frame forward.
 */
void video_player::next_frame() {
    update_frame(current_frame + 1);
}

/**
 * @brief video_player::next_frame
 * Moves the playback one frame backward.
 */
void video_player::previous_frame() {
    update_frame(current_frame - 1);
}

/**
 * @brief video_player::update_frame
 * @param frame_nbr
 * Updates the current frame if frame_nbr is valid.
 */
void video_player::update_frame(int frame_nbr) {
    if (set_playback_frame(frame_nbr)) {
        capture.set(CAP_PROP_POS_FRAMES, frame_nbr);
        capture.read(frame);
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
        show_frame();
    }
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
        video_overlay->undo(capture.get(CV_CAP_PROP_POS_FRAMES));
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
        video_overlay->clear(capture.get(CV_CAP_PROP_POS_FRAMES));
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
 * Starts drawing on the overlay, if the overlay is visible
 * and the video is loaded and paused.
 * If the video is paused, the frame in the GUI is updated.
 * @param pos Mouse coordinates.
 */
void video_player::video_mouse_pressed(QPoint pos) {
    if (capture.isOpened() && is_paused()) {
        scale_position(pos);
        if (choosing_zoom_area) {
            zoom_area->set_start_pos(pos);
            zoom_area->update_drawing_pos(pos);
        } else {
            video_overlay->mouse_pressed(pos, capture.get(CV_CAP_PROP_POS_FRAMES));
        }
        update_overlay();
    }
}

/**
 * @brief video_player::video_mouse_released
 * Ends drawing on the overlay, if the overlay is visible
 * and the video is loaded and paused.
 * If the video is paused, the frame in the GUI is updated.
 * @param pos Mouse coordinates.
 */
void video_player::video_mouse_released(QPoint pos) {
    if (capture.isOpened() && is_paused()) {
        scale_position(pos);
        if (choosing_zoom_area) {
            zoom_area->update_drawing_pos(pos);
            zoom_area->choose_area();
            choosing_zoom_area = false; // Reset the mode. You can only choose a zoom area during one drag with the mouse.
        }
        video_overlay->mouse_released(pos, capture.get(CV_CAP_PROP_POS_FRAMES));
        update_overlay();
    }
}

/**
 * @brief video_player::video_mouse_moved
 * Updates drawing on the overlay, if the overlay is visible
 * and the video is loaded and paused.
 * If the video is paused, the frame in the GUI is updated.
 * @param pos Mouse coordinates.
 */
void video_player::video_mouse_moved(QPoint pos) {
    if (capture.isOpened() && is_paused()) {
        scale_position(pos);
        if (choosing_zoom_area) {
            zoom_area->update_drawing_pos(pos);
        } else {
            video_overlay->mouse_moved(pos, capture.get(CV_CAP_PROP_POS_FRAMES));
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
    // Calculate the coordinates on the actual video frame from
    // the coordinates in the window the video is playing in.
    double scalex = (double) video_frame_width/frame_width;
    double scaley = (double) video_frame_height/frame_height;
    pos.setX(scalex*pos.x());
    pos.setY(scaley*pos.y());
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
    path_to_folder.append(QString::number(capture.get(CV_CAP_PROP_POS_FRAMES)));
    path_to_folder.append(".tiff");

    QImageWriter writer(path_to_folder, "tiff");
    writer.write(img);
}
