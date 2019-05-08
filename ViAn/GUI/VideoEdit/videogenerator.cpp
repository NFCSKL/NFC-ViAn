#include "videogenerator.h"

#include "constants.h"
#include "Project/video.h"
#include "Video/framemanipulator.h"
#include "videoedititem.h"
#include "videointerval.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"

#include <QCoreApplication>
#include <QListWidget>
#include <QDebug>

VideoGenerator::VideoGenerator(QListWidget* list, QString name, QSize size,
                               int fps, bool keep_size, bool title_screen,
                               QString title_text, QString description, int duration) {
    m_list = list;
    m_name = name;
    m_size = size;
    m_fps = fps;
    m_keep_size = keep_size;
    m_title = title_screen;
    m_title_text = title_text;
    m_description = description;
    m_duration = duration;
}

void VideoGenerator::generate_video() {
    cv::Mat frame;
    cv::VideoWriter vw;
    int fourCC = CV_FOURCC('M', 'J', 'P', 'G');
    cv::Size cv_size(m_size.width(), m_size.height());
    vw.open(m_name.toStdString(), fourCC, m_fps, cv_size);
    if (!vw.isOpened()) {
        qWarning() << "vw not opened";
        return;
    }

    // Create title screen
    if (m_title) {
        cv::Mat black = cv::Mat::zeros(cv_size, CV_8UC1);
        cvtColor(black, black, CV_GRAY2RGB);

        std::string text = m_title_text.toStdString();
        std::string desc = m_description.toStdString();
        auto font = cv::FONT_HERSHEY_TRIPLEX;
        auto desc_font = cv::FONT_HERSHEY_DUPLEX;
        double fontscale = 5;
        double desc_fontscale = fontscale-4;
        int thickness = 2;
        int baseline = 0;
        cv::Size title_size = cv::getTextSize(text, font, fontscale,
                                             thickness, &baseline);
        cv::Size desc_size = cv::getTextSize(desc, desc_font, desc_fontscale,
                                             thickness, &baseline);
        int title_x = (black.cols - title_size.width) / 2;
        int title_y = (black.rows + title_size.height) / 2;
        int desc_x = (black.cols - desc_size.width) / 2;
        int desc_y = title_y + desc_size.height*2;

        cv::putText(black, text, cv::Point(title_x, title_y),
                    font, fontscale, cv::Scalar(255,255,255), thickness);
        cv::putText(black, desc, cv::Point(desc_x, desc_y),
                    desc_font, desc_fontscale, cv::Scalar(255,255,255), thickness);
        // Add title screen for 5 sec
        for (int f = 0; f < m_fps*m_duration; f++) {
            vw.write(black);
        }
    }

    // Loop over all items
    for (int i = 0; i < m_list->count() && !m_aborted; ++i) {
        VideoEditItem* ve_item = dynamic_cast<VideoEditItem*>(m_list->item(i));
        VideoState state;
        state = ve_item->get_interval()->get_state();

        emit update_text("Generating video... " + QString::number(i+1)
                         + "/" + QString::number(m_list->count()));
        emit update_range(ve_item->get_start(), ve_item->get_end());


        cv::VideoCapture capture(ve_item->get_path().toStdString());
        if (!capture.isOpened()) continue;
        capture.set(CV_CAP_PROP_POS_FRAMES, ve_item->get_start());

        // Update color corrections
        FrameManipulator manipulator;
        manipulator.set_brightness(state.brightness);
        manipulator.set_contrast(state.contrast);
        manipulator.set_gamma(state.gamma);

        // Update rotation
        int rotation = state.rotation;
        if (rotation == Constants::DEGREE_MIN) {
            rotation = -1;
        } else if (rotation == Constants::DEGREE_90) {
            rotation = cv::ROTATE_90_CLOCKWISE;
        } else if (rotation == Constants::DEGREE_180) {
            rotation = cv::ROTATE_180;
        } else if (rotation == Constants::DEGREE_270) {
            rotation = cv::ROTATE_90_COUNTERCLOCKWISE;
        }

        // Update flip
        int flip = 5;       // 5 means no flip
        bool flip_h = state.flip_h;
        bool flip_v = state.flip_v;
        if (rotation == cv::ROTATE_90_CLOCKWISE || rotation == cv::ROTATE_90_COUNTERCLOCKWISE) {
            std::swap(flip_h, flip_v);
        }
        if (flip_h && flip_v) {
            flip = -1;
        } else if (flip_h) {
            flip = 0;
        } else if (flip_v) {
            flip = 1;
        }

        int width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
        int height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
        if (rotation == cv::ROTATE_90_CLOCKWISE || rotation == cv::ROTATE_90_COUNTERCLOCKWISE) {
            std::swap(width, height);
        }

        bool width_matter = (width - m_size.width() >= height - m_size.height());
        int max_dim = (width_matter) ? width : height;
        float scale = (width_matter) ? static_cast<float>(m_size.width()) / max_dim
                                     : static_cast<float>(m_size.height()) / max_dim;

        // Iterate over the interval
        while (capture.get(CV_CAP_PROP_POS_FRAMES) <= ve_item->get_end() && !m_aborted) {
            int curr_frame = static_cast<int>(capture.get(CV_CAP_PROP_POS_FRAMES));
            update_progress(curr_frame);
            QCoreApplication::processEvents();
            if (!capture.read(frame)) break;

            if (frame.type() == CV_8UC1) {
                cvtColor(frame, frame, CV_GRAY2RGB);
            }

            // Set state variables to frame
            manipulator.apply(frame);
            if (rotation != -1) cv::rotate(frame, frame, rotation);
            if (flip != 5) cv::flip(frame, frame, flip);

            cv::Mat resized = cv::Mat::zeros(cv_size, frame.type());
            cv::Rect roi;
            if (m_keep_size) {
                roi.width = width;
                roi.height = height;
                roi.x = (m_size.width() - roi.width) / 2;
                roi.y = (m_size.height() - roi.height) / 2;
            } else if (width_matter) {
                roi.width = m_size.width();
                roi.x = 0;
                roi.height = height * scale;
                roi.y = (m_size.height() - roi.height) / 2;
            } else {
                roi.y = 0;
                roi.height = m_size.height();
                roi.width = width * scale;
                roi.x = (m_size.width() - roi.width) / 2;
            }
            cv::resize(frame, resized(roi), roi.size());

            vw.write(resized);
        }
        capture.release();
    }
    vw.release();
    emit finished();
}

void VideoGenerator::abort() {
    m_aborted = true;
}
