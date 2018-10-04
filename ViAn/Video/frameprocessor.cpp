#include "frameprocessor.h"
#include <QDebug>
#include <QTime>
#include "utility.h"

const int FrameProcessor::DEGREES_0 = 0;
const int FrameProcessor::DEGREES_90 = 90;
const int FrameProcessor::DEGREES_180 = 180;
const int FrameProcessor::DEGREES_270 = 270;

FrameProcessor::FrameProcessor(std::atomic_bool* new_frame, std::atomic_bool* changed,
                               zoomer_settings* z_settings, std::atomic_int* width, std::atomic_int* height,
                               std::atomic_bool* new_frame_video, manipulation_settings* m_settings, video_sync* v_sync,
                               std::atomic_int* frame_index, overlay_settings* o_settings, std::atomic_bool* overlay_changed, std::atomic_bool *abort) {
    m_new_frame = new_frame;

    m_overlay_changed = overlay_changed;
    m_changed = changed;
    m_z_settings = z_settings;
    m_man_settings = m_settings;
    m_o_settings = o_settings;
    m_v_sync = v_sync;
    m_new_frame_video = new_frame_video;

    m_width = width;
    m_height = height;

    m_frame_index = frame_index;
    m_abort = abort;
}

FrameProcessor::~FrameProcessor() {}

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
    while (!m_abort->load()) {
        std::unique_lock<std::mutex> lk(m_v_sync->lock);
        m_v_sync->con_var.wait(lk, [&]{return m_abort->load() || m_new_frame->load() || m_changed->load() || m_new_frame_video->load() || m_overlay_changed->load();});
        if (m_abort->load()) {
            lk.unlock();
            break;
        }

        // A new video has been loaded. Reset processing settings
        if (m_new_frame_video->load()) {
            reset_settings();
            m_overlay = m_o_settings->overlay;
            m_o_settings->overlay_removed = false;
            update_overlay_settings();
            update_manipulator_settings();
            update_zoomer_settings();

            m_unrotated_size = cv::Size(0, 0);
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
            if (!m_new_frame->load() && !m_z_settings->skip_frame_refresh) {
                process_frame();
                lk.unlock();
                continue;
            } else {
                m_z_settings->skip_frame_refresh = false;
            }
        }

        // A new frame has been loaded by the VideoPlayer
        if (m_new_frame->load() && m_overlay) {
            if (m_z_settings->set_state) {
                load_zoomer_state();
            }
            m_new_frame->store(false);
            try {
                m_frame = m_v_sync->frame.clone();
            } catch (cv::Exception& e) {
                qWarning() << "Failed to copy new frame";
                emit set_play_btn(false);
                lk.unlock();
                m_v_sync->con_var.notify_all();
                continue;
            }

            // Check for new dimensions on unrotated frame
            // update zoomer as necessary
            // This is required for image sequences since
            // they can contain images of various sizes
            cv::Rect prev_size = m_zoomer.get_frame_rect();
            int new_width{m_frame.cols}, new_height{m_frame.rows};
            bool has_new_frame_size{m_unrotated_size.width != new_width || m_unrotated_size.height != new_height};
            bool frame_rect_changed{prev_size.width != new_width || prev_size.height != new_height};

            if (frame_rect_changed && has_new_frame_size) {
                m_zoomer.set_frame_size(cv::Size(new_width, new_height));
                if (has_new_zoom_state) {
                    m_zoomer.enforce_frame_boundaries();
                } else {
                    m_zoomer.fit_viewport();
                }
                emit_zoom_data();
            }
            has_new_zoom_state = false;
            m_unrotated_size = cv::Size(new_width, new_height);
            process_frame();

            lk.unlock();
            m_v_sync->con_var.notify_all();
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
    cv::Mat manipulated_frame;
    try {
        manipulated_frame = m_frame.clone();
    } catch (cv::Exception& e) {
        qWarning() << "Failed to copy new frame in processor";
        return;
    }

    // Rotates the frame, according to the choosen direction.
    // TODO: Computation heavy.. Will cause lag on larger images.
    // Should ideally be applied after zoom
//    if (ROTATE_MIN <= m_rotate_direction && m_rotate_direction <= ROTATE_MAX) {
//        cv::rotate(manipulated_frame, manipulated_frame, m_rotate_direction);
//    }

    // Create zoom perview mat
    cv::Mat preview_frame = manipulated_frame.clone();
    std::pair<double, double> ratios = Utility::size_ratio(m_z_settings->preview_window_size,
                                                           m_zoomer.get_transformed_size());
    double factor{std::min(ratios.first, ratios.second)};
    cv::resize(preview_frame, preview_frame, cv::Size(), factor, factor);
    cv::Rect view_rectangle = m_zoomer.get_view_rect(0);
    cv::rectangle(preview_frame, view_rectangle.tl() * factor,
                  view_rectangle.br() * factor, cv::Scalar(0,0,0), 2);
    cv::rectangle(preview_frame, view_rectangle.tl() * factor,
                  view_rectangle.br() * factor, cv::Scalar(255,255,255));

    int frame_num = m_frame_index->load();

    // Draws the text drawings on the overlay
    m_overlay->draw_overlay(manipulated_frame, frame_num);

    qDebug() << "SIZE before" << manipulated_frame.cols << manipulated_frame.rows;

    // Apply zoom
    m_zoomer.scale_frame(manipulated_frame);

    // Draws the other drawings on the overlay
    m_overlay->draw_overlay_scaled(manipulated_frame, frame_num, Utility::from_qpoint(m_z_settings->anchor), m_z_settings->zoom_factor);

    // Applies brightness and contrast
    m_manipulator.apply(manipulated_frame);

    qDebug() << "SIZE" << manipulated_frame.cols << manipulated_frame.rows;

    // Why not?
    if (ROTATE_MIN <= m_rotate_direction && m_rotate_direction <= ROTATE_MAX) {
        cv::rotate(manipulated_frame, manipulated_frame, m_rotate_direction);
    }

    //m_zoomer.scale_frame(manipulated_frame);

    qDebug() << "SIZE 2" << manipulated_frame.cols << manipulated_frame.rows;


    // Emit manipulated frame and current frame number
    emit zoom_preview(preview_frame);
    emit done_processing(m_frame, manipulated_frame, m_frame_index->load());
}

/**
 * @brief FrameProcessor::load_zoomer_state
 */
void FrameProcessor::load_zoomer_state() {
    m_z_settings->set_state = false;
    m_zoomer.load_state(m_z_settings->center, m_z_settings->zoom_factor, m_z_settings->rotation);

    if (m_z_settings->rotation == DEGREES_0) {
        m_rotate_direction = ROTATE_NONE;
    } else if (m_z_settings->rotation == DEGREES_90) {
        m_rotate_direction = ROTATE_90;
    } else if (m_z_settings->rotation == DEGREES_180) {
        m_rotate_direction = ROTATE_180;
    } else if (m_z_settings->rotation == DEGREES_270) {
        m_rotate_direction = ROTATE_270;
    }
    has_new_zoom_state = true;
}

/**
 * @brief FrameProcessor::emit_zoom_data
 * Emitts all signals that sends zoom data
 */
void FrameProcessor::emit_zoom_data() {
    // Store changes made
    m_z_settings->zoom_factor = m_zoomer.get_scale_factor();
    m_z_settings->anchor = m_zoomer.get_anchor();

    emit set_zoom_state(m_zoomer.get_center(), m_zoomer.get_scale_factor(), m_zoomer.get_angle());
    emit set_anchor(m_zoomer.get_anchor());
    emit set_scale_factor(m_zoomer.get_scale_factor());
    emit set_rotation(m_zoomer.get_angle());

}

/**
 * @brief FrameProcessor::update_zoomer_settings
 * Updates the settings for the zoomer.
 * Modifies data from a shared pointer and should only be called after the v_sync_lock has been acquired
 */
void FrameProcessor::update_zoomer_settings() {
    // Viewport has changed size
    if (m_zoomer.get_viewport_size() != m_z_settings->draw_area_size) {
        m_zoomer.set_viewport_size(m_z_settings->draw_area_size);
    }
    // Set a new state to the zoomer, that means (currently) a new anchor and scale_factor
    else if (m_z_settings->set_state) {
        load_zoomer_state();
    }
    // Center the zoom rect
    else if (m_z_settings->do_point_zoom) {
        m_z_settings->do_point_zoom = false;
        m_zoomer.point_zoom(m_z_settings->center, m_z_settings->zoom_step);
    }
    // Scale/zoom factor has been changed
    else if (m_zoomer.get_scale_factor() != m_z_settings->zoom_factor) {
        m_zoomer.set_scale_factor(m_z_settings->zoom_factor);
    }
    // Zoom rectangle has changed
    else if (m_z_settings->has_new_zoom_area) {
        m_zoomer.area_zoom(m_z_settings->zoom_area_tl, m_z_settings->zoom_area_br);
        m_z_settings->has_new_zoom_area = false;
    }
    // Panning occured
    else if (m_z_settings->x_movement != 0 || m_z_settings->y_movement != 0) {
        m_zoomer.translate_viewport_center(m_z_settings->x_movement, m_z_settings->y_movement);
        m_z_settings->x_movement = 0;
        m_z_settings->y_movement = 0;
    }
    // Fit to screen
    else if (m_z_settings->fit) {
        m_z_settings->fit = false;
        m_zoomer.fit_viewport();
    }
    // Set original size (no zoom)
    else if (m_z_settings->original) {
        m_z_settings->original = false;
        m_zoomer.reset();
    }
    // Set interpolation method
    else if (m_zoomer.get_interpolation_method() != m_z_settings->interpolation){
        m_zoomer.set_interpolation_method(m_z_settings->interpolation);
    }

    emit_zoom_data();
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

    int rotate_direction = m_rotate_direction;
    if (m_man_settings->rotate == 1) {
        rotate_direction = (m_rotate_direction + (ROTATE_NUM + 1)) % ROTATE_NUM;
    } else if (m_man_settings->rotate == -1) {
        rotate_direction = (m_rotate_direction + (ROTATE_NUM - 1)) % ROTATE_NUM;
    }
    update_rotation(rotate_direction);
    m_man_settings->rotate = 0;
    emit set_zoom_state(m_zoomer.get_center(), m_zoomer.get_scale_factor(), m_zoomer.get_angle());
    if (m_man_settings->update_state) {
        emit set_bri_cont(m_manipulator.get_brightness(), m_manipulator.get_contrast());
    }
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
        QPoint text_point = m_zoomer.get_center();
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

void FrameProcessor::update_rotation(const int& direction) {
    //qDebug() << "m_rotatte_direction" << m_rotate_direction; // 0-3
    
    
    //qDebug() << "current rotation" << m_z_settings->rotation;
    if (direction != m_rotate_direction) {
        m_rotate_direction = direction;

        if (m_rotate_direction == ROTATE_NONE) {
            m_zoomer.update_rotation(DEGREES_0);
        } else if (m_rotate_direction == ROTATE_90) {
            m_zoomer.update_rotation(DEGREES_90);
        } else if (m_rotate_direction == ROTATE_180) {
            m_zoomer.update_rotation(DEGREES_180);
        } else if (m_rotate_direction == ROTATE_270) {
            m_zoomer.update_rotation(DEGREES_270);
        }
        m_zoomer.enforce_frame_boundaries();
    }
}

/**
 * @brief FrameProcessor::reset_settings
 * Resets the processing settings.
 * Should be used after a new video has been loaded by the VideoPlayer.
 * (Modifies shared data. Use v_sync_lock)
 */
void FrameProcessor::reset_settings() {
    m_new_frame_video->store(false);
    m_rotate_direction = ROTATE_NONE;

    // Reset manipulator values
    m_manipulator.reset();
    m_zoomer.set_angle(0);
    m_zoomer.set_frame_size(cv::Size(m_width->load(), m_height->load()));
    m_zoomer.reset();

    emit set_anchor(m_zoomer.get_anchor());
    emit set_scale_factor(m_zoomer.get_scale_factor());

}
