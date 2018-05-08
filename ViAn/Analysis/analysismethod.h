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
#include <atomic>
#include "utility.h"

using Settings = std::map<std::string,int>;
using SettingsDescr = std::map<std::string,std::string>;

/**
 * @brief The AnalysisMethod class
 * This class intended to use as Interface for all analysis types.
 * It's implementation is limited to saving rectangles and time intervals for
 * detection.
 */
class AnalysisMethod : public QObject, public QRunnable{
    Q_OBJECT    
    std::string m_save_path;            // Save path for finished analysis
    std::string m_source_file;          // Source video file used by opencv capture
    std::string m_ana_name;             // The name of the analysis

    bool m_scaling_done = false;        // Control states
    int prev_detection_frame = -1;
    bool detecting = false;
    bool paused = false;

protected:
    bool scaling_needed = false;

    const int FULL_HD_WIDTH = 1920;
    const int FULL_HD_HEIGHT = 1080;

    int num_frames = -1;            // Total number of video frames
    unsigned int sample_freq = 1;   // Sample frequency of analysis
    int current_frame_index = 0;    // The current frame number

    int scaled_width = -1;          // Width after scaling
    int scaled_height = -1;         // Height after scaling
    float scaling_ratio = 1.0;      // Scaling factor

    cv::VideoCapture capture;       // Video source

    cv::Mat analysis_frame, original_frame;
    Analysis m_analysis;
protected:
    // Implement this in subclass,
    // using set_setting(Var,value) for all desired settings
    //virtual void init_settings() = 0;
    // Implement this, return constant names, default value and descriptions
    // like so std::vector<std::tuple<name,int value, description>>

    //virtual void add_setting(const std::string& var, int value_default, const std::string& descr);

    void calculate_scaling_factor();
    void scale_frame();
public:
    AnalysisMethod(const std::string &video_path, const std::string& save_path, AnalysisSettings *settings);

    AnalysisSettings* analysis_settings = nullptr;

    //std::string get_descr(const std::string& var_name);          // Get variable description
    int get_setting(const std::string& var);             // Get integer value for variable
    //virtual void set_setting(const std::string& var, int value); // Set singled variable
    //virtual void set_full_settings(Settings settings);           // Set all variables
    //virtual std::vector<std::string> get_var_names();            // Get all variable names
    virtual void setup_analysis() = 0;
    virtual std::vector<DetectionBox> analyse_frame() = 0;

    int get_progress(int start_frame);  // Returns progress of analysis 1-100%
    cv::Rect get_bounding_box() const;
    void set_bounding_box(const cv::Rect &value); // Sets bounding box to analyse in video
    std::pair<int, int> get_interval() const;
    void set_interval(const std::pair<int, int> &value);
    std::string save_path() const;

    void set_analysis_area(cv::Rect area);

    bool sample_current_frame(); // Check if current frame is to be sampled
    bool* aborted = nullptr;    // Bool checked if analysis should be aborted

public slots:
    void run();
signals:
    void analysis_aborted();
    void send_progress(int progress);
    void finito(void);
    void finished_analysis(AnalysisProxy);
};

#endif // ANALYSISMETHOD_H
