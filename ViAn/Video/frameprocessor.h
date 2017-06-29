#ifndef FRAMEPROCESSOR_H
#define FRAMEPROCESSOR_H

#include <QObject>
#include <QMutex>
#include <opencv2/core/core.hpp>
#include "framemanipulator.h"
#include "zoomer.h"

class FrameProcessor : public QObject {
    Q_OBJECT
    cv::Mat m_frame;
    int m_rot_dir, m_pos;
    FrameManipulator* m_f_man;
    Zoomer* m_zoomer;
    QMutex* m_mutex;
    cv::VideoCapture* m_capture;
public:
    FrameProcessor(cv::Mat& frame, int rot_dir, int pos, FrameManipulator* f_man, Zoomer* zoomer, QMutex* mutex, cv::VideoCapture* capture);
public slots:
    void process();
signals:
    void done_processing(cv::Mat frame);
    void done(void);
    void frame_num(int num);
};

#endif // FRAMEPROCESSOR_H
