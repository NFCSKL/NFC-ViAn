#include "frameprocessor.h"
#include <QDebug>
#include <QTime>

FrameProcessor::FrameProcessor() {}

/**
 * @brief FrameProcessor::on_new_frame
 * Slot function for recieving a new frame
 * @param frame : mat object containing the frame
 * @param frame_index   : index of the frame
 */
void FrameProcessor::on_new_frame(cv::Mat frame, int frame_index) {
    m_frame = frame.clone();
    m_frame_index = frame_index;
    process_frame();
}

/**
 * @brief FrameProcessor::on_video_info
 * Slot function that sets the frame size for the zoomer
 * @param frame_width   :   width of the frames from the currently loaded video
 * @param frame_height  : height of the frames from the currently loaded video
 */
void FrameProcessor::on_video_info(int frame_width, int frame_height, int, int) {
    m_rotate_direction = ROTATE_NONE;
    m_zoomer.reset();
    m_manipulator.reset();

    m_zoomer.set_frame_size(cv::Size(frame_width, frame_height));
    m_zoomer.fit_viewport();
    set_scale_factor(m_zoomer.get_scale_factor());
    set_anchor(m_zoomer.get_anchor());
}

/**
 * @brief FrameProcessor::on_set_draw_area_size
 * Slot function that updates the zoomer about the drawing area size
 * @param size  : size of the area where the frame will be shown
 */
void FrameProcessor::on_set_draw_area_size(QSize size){
    m_zoomer.set_viewport_size(size);
}

/**
 * @brief FrameProcessor::on_set_zoom_rect
 * Slot function that updates the zoom rect
 * @param p1 : top-left corner of the rectangle
 * @param p2 : top-right corner of the rectangle
 */
void FrameProcessor::on_set_zoom_rect(QPoint p1, QPoint p2) {
    m_zoomer.set_zoom_rect(p1, p2);
    set_scale_factor(m_zoomer.get_scale_factor());
    set_anchor(m_zoomer.get_anchor());
    process_frame();
}

/**
 * @brief FrameProcessor::on_zoom_out
 * Slot function that decreases the zoom
 */
void FrameProcessor::on_zoom_out(){
    m_zoomer.set_scale_factor(m_zoomer.get_scale_factor() * 0.5);
    set_scale_factor(m_zoomer.get_scale_factor());
    set_anchor(m_zoomer.get_anchor());
    process_frame();
}

/**
 * @brief FrameProcessor::on_fit_screen
 * Slot function for fitting the frame to the display area
 */
void FrameProcessor::on_fit_screen(){
    m_zoomer.fit_viewport();
    set_scale_factor(m_zoomer.get_scale_factor());
    set_anchor(m_zoomer.get_anchor());
    process_frame();
}

/**
 * @brief FrameProcessor::on_original_size
 * Slot function for resizing the frame to its original size
 */
void FrameProcessor::on_original_size() {
    m_zoomer.reset();
    set_scale_factor(1);
    set_anchor(QPoint(0,0));
    process_frame();
}

/**
 * @brief FrameProcessor::on_resize
 * Slot function that updated the frame when it's resized
 */
void FrameProcessor::on_resize() {
    m_zoomer.set_scale_factor(m_zoomer.get_scale_factor());
    set_scale_factor(m_zoomer.get_scale_factor());
    set_anchor(m_zoomer.get_anchor());
    process_frame();
}

/**
 * @brief FrameProcessor::on_rotate_right
 * Slot function that rotates the frame 90 degrees to the right
 */
void FrameProcessor::on_rotate_right(){
    m_rotate_direction = (m_rotate_direction + 1) % ROTATE_NUM;
    m_zoomer.flip();
    process_frame();
}

/**
 * @brief FrameProcessor::on_rotate_left
 * Slot function that rotates the frame 90 degrees to the left
 */
void FrameProcessor::on_rotate_left(){
    m_rotate_direction = (m_rotate_direction + (ROTATE_NUM - 1)) % ROTATE_NUM;
    m_zoomer.flip();
    process_frame();
}

/**
 * @brief FrameProcessor::on_set_bright_cont
 * Slot function that updates the brightness and contrast values
 * @param b_val
 * @param c_val
 */
void FrameProcessor::on_set_bright_cont(int b_val, double c_val){
    m_manipulator.set_brightness(b_val);
    m_manipulator.set_contrast(c_val);
    process_frame();
}

/**
 * @brief FrameProcessor::on_move_zoom_rect
 * Slot function that updates the position of the zoom rectangle
 * @param x : movement along the x-axis
 * @param y : movement along the y-axis
 */
void FrameProcessor::on_move_zoom_rect(int x, int y) {
    m_zoomer.move_zoom_rect(x, y);
    set_anchor(m_zoomer.get_anchor());
    process_frame();
}

/**
 * @brief FrameProcessor::process_frame
 * Applies various manipulations to the frame currently held by the class.
 * These manipulations consists of rotation, zoom and brightness/contrast changes.
 * When done it will emit the manipulated frame on the done_processing signal.
 */
void FrameProcessor::process_frame() {

    if (m_frame.empty()) return;

    cv::Mat manipulated_frame = m_frame.clone();

    // Rotates the frame, according to the choosen direction.
    // If direction is in the valid range the frame is rotated.
    if (ROTATE_MIN <= m_rotate_direction && m_rotate_direction <= ROTATE_MAX) {
        cv::rotate(manipulated_frame, manipulated_frame, m_rotate_direction);
    }

    // Scales the frame
    m_zoomer.scale_frame(manipulated_frame);

    // Applies brightness and contrast
    m_manipulator.apply(manipulated_frame);

    // Emit manipulated frame and current frame number
    done_processing(manipulated_frame, m_frame_index);
}
