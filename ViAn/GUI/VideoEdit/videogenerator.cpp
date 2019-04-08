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
                               int fps, bool keep_size) {
    m_list = list;
    m_name = name;
    m_size = size;
    m_fps = fps;
    m_keep_size = keep_size;
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

    // Loop over all items
    for (int i = 0; i < m_list->count() && !m_aborted; ++i) {
        VideoEditItem* ve_item = dynamic_cast<VideoEditItem*>(m_list->item(i));
        VideoState state;
        state = ve_item->get_interval()->get_state();

        emit update_text("Generating video... " + QString::number(i)
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
