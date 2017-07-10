#include "frameprocessor.h"
#include <QDebug>
#include <QThread>
#include <QTime>

FrameProcessor::FrameProcessor(cv::Mat& frame, int rot_dir, int pos, FrameManipulator* f_man, Zoomer* zoomer, QMutex* mutex, cv::VideoCapture* capture) {
    mutex->lock();
    m_frame = frame.clone();
    mutex->unlock();
    m_rot_dir = rot_dir;
    m_f_man = f_man;
    m_zoomer = zoomer;
    m_mutex = mutex;
    m_capture = capture;
    m_pos = pos;
}

void FrameProcessor::process() {
    m_mutex->lock();
    m_capture->set(CV_CAP_PROP_POS_FRAMES, m_pos);
    m_capture->read(m_frame);
    cv::rotate(m_frame, m_frame, m_rot_dir);
    // Scales the frame
    if (m_zoomer->get_scale_factor() != 1) m_zoomer->scale_frame(m_frame);
    // Applies brightness and contrast
    m_f_man->apply(m_frame);
    // Emit manipulated frame and current frame number
    emit done_processing(m_frame.clone());
    emit frame_num(m_capture->get(CV_CAP_PROP_POS_FRAMES) - 1);
    m_frame.release();
    m_mutex->unlock();
    emit done();
}
