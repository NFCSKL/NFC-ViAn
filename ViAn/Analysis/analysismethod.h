#ifndef ANALYSISMETHOD_H
#define ANALYSISMETHOD_H
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QRunnable>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "Project/Analysis/analysis.h"
#include "analysissettings.h"
#include "Project/Analysis/analysisproxy.h"
#include "Filehandler/saveable.h"
using Settings = std::map<std::string,int>;
using SettingsDescr = std::map<std::string,std::string>;
class AnalysisMethod : public QObject ,public QRunnable{
    Q_OBJECT    
    Settings m_settings;
    SettingsDescr m_descriptions;
    std::string m_save_path;
    std::string m_source_file;
    bool m_scaling_done = false;
protected:
    AnalysisInterval interval;
    cv::Rect bounding_box;
    bool use_interval = false;
    bool use_bounding_box = false;

    // Implement this in subclass,
    // using set_setting(Var,value) for all desired settings
    virtual void init_settings() = 0;
    // Implement this, return constant names, default value and descriptions
    // like so std::vector<std::tuple<name,int value, description>>

    virtual void add_setting(const std::string& var, int value_default, const std::string& descr);


public:
    std::string get_descr(const std::string& var_name);
    virtual int get_setting(const std::string& var);
    virtual void set_setting(const std::string& var, int value);
    virtual std::vector<std::string> get_var_names();

    AnalysisMethod(const std::string &video_path, const std::string& save_path);
    void abort_analysis();
    void pause_analysis();
    void set_include_exclude_area(std::vector<cv::Point> points, bool exclude_polygon);
    void set_analysis_area(cv::Rect area);
    virtual void setup_analysis() = 0;
    virtual std::vector<DetectionBox> analyse_frame() = 0;

    bool sample_current_frame();
    bool load_video();


    int get_progress(int start_frame);

    void setBounding_box(const cv::Rect &value);

    AnalysisInterval getInterval() const;
    void setInterval(const AnalysisInterval &value);

    std::string save_path() const;

private:
    int prev_detection_frame = -1;
    bool detecting = false;
    bool paused = false;            // Control states
    bool aborted = false;

protected:
    const int FULL_HD_WIDTH = 1920;
    const int FULL_HD_HEIGHT = 1080;

    int num_frames = -1;
    unsigned int sample_freq = 1;
    int current_frame_index = 0;    // The current frame number

    int scaled_width = -1;
    int scaled_height = -1;
    float scaling_ratio = 1.0;

    cv::VideoCapture capture;       // Video source
    cv::Mat analysis_frame, exclude_frame, original_frame;   // The frame fetched last
    Analysis m_analysis;

    bool do_exclusion = false;
    bool scaling_needed = false;
    bool m_exclude_polygon;
    std::vector<cv::Point> include_exclude_poly;

    void calculate_scaling_factor();
    void scale_frame();
public slots:
    void run();
signals:
    void send_progress(int progress);
    void finito(void);
    void finished_analysis(AnalysisProxy);
};

#endif // ANALYSISMETHOD_H
