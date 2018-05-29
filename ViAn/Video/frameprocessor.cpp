#include "frameprocessor.h"
#include <QDebug>
#include <QTime>
#include "utility.h"

FrameProcessor::FrameProcessor(std::atomic_bool* new_frame, std::atomic_bool* changed,
                               zoomer_settings* z_settings, std::atomic_int* width, std::atomic_int* height,
                               std::atomic_bool* new_video, manipulation_settings* m_settings, video_sync* v_sync,
                               std::atomic_int* frame_index, overlay_settings* o_settings, std::atomic_bool* overlay_changed) {
    m_new_frame = new_frame;

    m_overlay_changed = overlay_changed;
    m_changed = changed;
    m_z_settings = z_settings;
    m_man_settings = m_settings;
    m_o_settings = o_settings;
    m_v_sync = v_sync;
    m_new_video = new_video;

    m_width = width;
    m_height = height;

    m_frame_index = frame_index;
    // NICLAS
    // cv::namedWindow("test");
}

/**
 * @brief FrameProcessor::check_events
 * This method handles all processing requests and will sleep when there is none.
 * It uses a conditional variable and waits for one of three bools (m_new_frame, m_changed, m_new_video).
 * Each bool will trigger a different behavior:
 * m_new_frame: copies and processes the current frame. This is done during ordinary playback
 * m_changed: indicates that one of the processing settings has been changed by the user and must be applied
 * m_new_video: a new video has been loaded, processing settings are restored
 *
 * OBS! Frameprocesser should never enter check_events during a load operation in video
 * player, this introduces a race condition where frameprocesser may use old frame with new
 * load settings
 */
void FrameProcessor::check_events() {
    while (true) {
        std::unique_lock<std::mutex> lk(m_v_sync->lock);
        m_v_sync->con_var.wait(lk, [&]{return m_new_frame->load() || m_changed->load() || m_new_video->load() || m_overlay_changed->load();});
        // A new video has been loaded. Reset processing settings    
        if (m_new_video->load()) {
            reset_settings();
            m_overlay = m_o_settings->overlay;
            m_o_settings->overlay_removed = false;

            lk.unlock();
            continue;
        }

        if (m_overlay == nullptr || m_o_settings->overlay_removed) {
            m_overlay_changed->store(false);
        }

        // The overlay has been changed by the user
        if (m_overlay_changed->load()) {
            m_overlay_changed->store(false);
            update_overlay_settings();
            // Skip reprocessing of old frame if there is a new
            if (!m_new_frame->load()) {
                process_frame();
            }
            lk.unlock();
            continue;
        }

        // Settings has been changed by the user
        if (m_changed->load()) {
            m_changed->store(false);
            update_manipulator_settings();
            update_zoomer_settings();

            // Skip reprocessing of old frame if there is a new
            if (!m_new_frame->load() && !skip_process) {
                process_frame();
                lk.unlock();
                continue;
            }
            skip_process = false;
        }

        // A new frame has been loaded by the VideoPlayer
        if (m_new_frame->load() && m_overlay) {
            m_new_frame->store(false);
            m_frame = m_v_sync->frame.clone();
            process_frame();

            lk.unlock();
            m_v_sync->con_var.notify_one();
            continue;
        }
    }

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
    if (ROTATE_MIN <= m_rotate_direction && m_rotate_direction <= ROTATE_MAX) {
        cv::rotate(manipulated_frame, manipulated_frame, m_rotate_direction);
    }

    // Displays the zoom rectangle on the original frame in a new windows.
    // NICLAS
    // cv::Mat tmp = manipulated_frame.clone();
    // cv::rectangle(tmp, m_zoomer.get_zoom_rect(), cv::Scalar(255,0,0));
    // cv::imshow("test", tmp);
    // Draws the overlay
    m_overlay->draw_overlay(manipulated_frame, m_frame_index->load());

    // Scales the frame
    m_zoomer.scale_frame(manipulated_frame);

    // Applies brightness and contrast
    m_manipulator.apply(manipulated_frame);

    // Emit manipulated frame and current frame number
    emit done_processing(m_frame, manipulated_frame, m_frame_index->load());
}

/**
 * @brief FrameProcessor::update_zoomer_settings
 * Updates the settings for the zoomer.
 * Modifies data from a shared pointer and should only be called after the v_sync_lock has been acquired
 */
void FrameProcessor::update_zoomer_settings() {
    // Viewport has changed size
    if (m_zoomer.get_viewport_size() != m_z_settings->draw_area_size) {
        qDebug() << "SIZE" << m_z_settings->draw_area_size;
        m_zoomer.set_viewport_size(m_z_settings->draw_area_size);
        m_zoomer.update_rect_size();
    }
    // Set a new state to the zoomer, that means (currently) a new anchor and scale_factor
    else if (m_z_settings->set_state) {
        // wait for frame rect to be set?
        qDebug() << "set state";
        m_z_settings->set_state = false;
        //skip_process = true;
        m_zoomer.set_state(m_z_settings->anchor, m_z_settings->zoom_factor);
    }
    // Center the zoom rect
    else if (m_z_settings->do_center) {
        m_z_settings->do_center = false;

        m_zoomer.center_zoom_rect(m_z_settings->center, m_z_settings->zoom_step);
    }
    // Scale/zoom factor has been changed
    else if (m_zoomer.get_scale_factor() != m_z_settings->zoom_factor) {
        m_zoomer.set_scale_factor(m_z_settings->zoom_factor);
    }
    // Zoom rectangle has changed
    else if (m_zoomer.get_zoom_rect() != cv::Rect(m_z_settings->zoom_tl.x(), m_z_settings->zoom_tl.y(),
                                                    m_z_settings->zoom_br.x(), m_z_settings->zoom_br.y())) {

        m_zoomer.set_zoom_rect(m_z_settings->zoom_tl, m_z_settings->zoom_br);
    }
    // Panning occured
    else if (m_z_settings->x_movement != 0 || m_z_settings->y_movement != 0) {
        m_zoomer.move_zoom_rect(m_z_settings->x_movement, m_z_settings->y_movement);
        m_z_settings->x_movement = 0;
        m_z_settings->y_movement = 0;
    }
    // Fit to screen
    else if (m_z_settings->fit) {
        m_z_settings->fit = false;
        m_zoomer.fit_viewport();
    }
    // Set original size (no zoom)
    else if (m_z_settings->original){
        m_z_settings->original = false;
        m_zoomer.reset();
    }

    // Store changes made
    m_z_settings->zoom_factor = m_zoomer.get_scale_factor();
    cv::Rect tmp = m_zoomer.get_zoom_rect();
    m_z_settings->zoom_tl = QPoint(tmp.x, tmp.y);
    m_z_settings->zoom_br = QPoint(tmp.width, tmp.height);

    emit set_anchor(m_zoomer.get_anchor());
    emit set_scale_factor(m_zoomer.get_scale_factor());
}

void FrameProcessor::update_state_settings() {
    // Set a new state to the zoomer, that means (currently) a new anchor and scale_factor
    if (m_z_settings->set_state) {
        // wait for frame rect to be set?
        qDebug() << "set state";
        m_z_settings->set_state = false;
        //skip_process = true;
        m_zoomer.set_state(m_z_settings->anchor, m_z_settings->zoom_factor);
    }

    // Store changes made
    m_z_settings->zoom_factor = m_zoomer.get_scale_factor();
    cv::Rect tmp = m_zoomer.get_zoom_rect();
    m_z_settings->zoom_tl = QPoint(tmp.x, tmp.y);
    m_z_settings->zoom_br = QPoint(tmp.width, tmp.height);

    emit set_anchor(m_zoomer.get_anchor());
    emit set_scale_factor(m_zoomer.get_scale_factor());
}

/**
 * @brief FrameProcessor::update_manipulator_settings
 * Updates manipulator and rotation settings.
 * Modifies data from a shared pointer and should therefor only be used after
 * the v_sync_lock has been acquired.
 */
void FrameProcessor::update_manipulator_settings() {
    m_manipulator.set_brightness(m_man_settings->brightness);
    m_manipulator.set_contrast(m_man_settings->contrast);

    if (m_man_settings->rotate == 1) {
        m_rotate_direction = (m_rotate_direction + 1) % ROTATE_NUM;
        m_zoomer.flip();
    } else if (m_man_settings->rotate == -1) {
        m_rotate_direction = (m_rotate_direction + (ROTATE_NUM - 1)) % ROTATE_NUM;
        m_zoomer.flip();
    }
    m_man_settings->rotate = 0;
}

/**
 * @brief FrameProcessor::update_overlay_settings
 *
 */
void FrameProcessor::update_overlay_settings() {
    int curr_frame = m_frame_index->load();
    m_overlay->set_showing_overlay(m_o_settings->show_overlay);
    m_overlay->set_tool(m_o_settings->tool);
    m_overlay->set_colour(m_o_settings->color);
    m_overlay->set_text_settings(m_o_settings->current_string, m_o_settings->current_font_scale);


    // Update text action
    if (m_o_settings->update_text) {
        m_o_settings->update_text = false;
        m_overlay->update_text(m_o_settings->text, m_o_settings->shape);
    // Clear drawings action
    } else if (m_o_settings->clear_drawings) {
        m_o_settings->clear_drawings = false;
        m_overlay->clear(m_o_settings->frame);
    // Delete the current drawing
    } else if (m_o_settings->delete_drawing) {
        m_o_settings->delete_drawing = false;
        m_overlay->delete_drawing(m_o_settings->shape);
    // Create a new text drawing
    } else if (m_o_settings->create_text) {
        int height = m_zoomer.get_zoom_rect().height;
        int width = m_zoomer.get_zoom_rect().width;
        int anchor_x = m_zoomer.get_anchor().x();
        int anchor_y = m_zoomer.get_anchor().y();
        QPoint text_point = QPoint(anchor_x+width/2, anchor_y+height/2);
        m_o_settings->create_text = false;
        m_overlay->create_text(text_point, curr_frame);
    // Mouse double clicked action
    } else if (m_o_settings->mouse_double_clicked) {
        m_o_settings->mouse_double_clicked = false;
        m_overlay->mouse_double_clicked(m_o_settings->pos, curr_frame);
    // Mouse pressed action
    } else if (m_o_settings->mouse_clicked) {
        m_o_settings->mouse_clicked = false;
        m_overlay->mouse_pressed(m_o_settings->pos, curr_frame, m_o_settings->right_click);
    // Mouse released action
    } else if (m_o_settings->mouse_released) {
        m_o_settings->mouse_released = false;
        m_overlay->mouse_released(m_o_settings->pos, curr_frame, m_o_settings->right_click);
    // Mouse moved action
    } else if (m_o_settings->mouse_moved) {
        m_o_settings->mouse_moved = false;
        m_overlay->mouse_moved(m_o_settings->pos, curr_frame, m_o_settings->shift_modifier, m_o_settings->ctrl_modifier);
    // Mouse scroll wheel action
    } else if (m_o_settings->mouse_scroll) {
        m_o_settings->mouse_scroll = false;
        m_overlay->mouse_scroll(m_o_settings->pos, curr_frame);
    // Set current drawing action
    } else if (m_o_settings->set_current_drawing) {
        m_o_settings->set_current_drawing = false;
        m_overlay->set_current_drawing(m_o_settings->shape);
    }
}

/**
 * @brief FrameProcessor::reset_settings
 * Resets the processing settings.
 * Should be used after a new video has been loaded by the VideoPlayer.
 * (Modifies shared data. Use v_sync_lock)
 */
void FrameProcessor::reset_settings() {
    m_new_video->store(false);
    m_rotate_direction = ROTATE_NONE;

    // Reset manipulator values
    m_manipulator.reset();
    m_man_settings->brightness = m_manipulator.BRIGHTNESS_DEFAULT;
    m_man_settings->contrast = m_manipulator.CONTRAST_DEFAULT;

    m_zoomer.set_frame_size(cv::Size(m_width->load(), m_height->load()));

    //if (skip_reset) {
        //qDebug() << "if";
        // The video state should already have been set elsewhere
        //m_z_settings->set_state = false;
    if (m_z_settings->do_reset) {
        qDebug() << "if";
        m_zoomer.reset();
        // Centers zoom rectangle and displays the frame without zoom
        m_zoomer.fit_viewport();

        // Store current zoomer settings to shared structure
        m_z_settings->zoom_factor = m_zoomer.get_scale_factor();
        cv::Rect tmp = m_zoomer.get_zoom_rect();
        m_z_settings->zoom_tl = QPoint(tmp.x, tmp.y);
        m_z_settings->zoom_br = QPoint(tmp.width, tmp.height);
    } else {
        // TODO Not sure bout this
        // Make a new set state function
        m_z_settings->set_state = true;
        update_zoomer_settings();
    }

    emit set_anchor(m_zoomer.get_anchor());
    emit set_scale_factor(m_zoomer.get_scale_factor());

}
