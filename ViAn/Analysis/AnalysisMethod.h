#ifndef ANALYSISMETHOD_H
#define ANALYSISMETHOD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/videoio/videoio.hpp"


class AnalysisMethod : public QThread {
public:
    //Analysis();
    bool load_video();

private:
    virtual void setup_analysis() = 0;
    virtual void do_analysis() = 0;

protected:
    bool paused = false;            // Control states
    bool aborted = false;
    unsigned int current_frame;     // The current frame number
    cv::VideoCapture capture;       // Video source
    cv::Mat frame;                  // The frame fetched last
    void run() override;

signals:
    void send_detection();          // Used to send the interval of detection

private slots:
    void on_start();                // Start or resume the analysis
    void on_pause();                // Pause the analysis
    void on_abort();                // Abort the analysis
};

#endif // ANALYSISMETHOD_H
