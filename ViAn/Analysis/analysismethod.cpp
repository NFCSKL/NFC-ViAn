#include "analysismethod.h"

#include "analysissettings.h"
#include "constants.h"
#include "Project/Analysis/analysisproxy.h"
#include "Project/Analysis/poi.h"
#include "utility.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <QDebug>

AnalysisMethod::AnalysisMethod(const std::string &video_path, const std::string& save_path, AnalysisSettings* settings) {
    analysis_settings = settings;
    m_source_file = video_path;
    std::size_t index = video_path.find_last_of('/') + 1;
    std::string vid_name = video_path.substr(index);
    //std::string vid_name = Utility::name_from_path(video_path);
    index = vid_name.find_last_of('.');
    vid_name = vid_name.substr(0,index);
    m_ana_name = vid_name + settings->get_type_string();
    m_save_path = save_path;
}

/**
 * @brief AnalysisMethod::sample_current_frame
 * Checks if the current frame is to be analysed.
 * @return true if the current frame should be analysed.
 */
bool AnalysisMethod::sample_current_frame() {
    return current_frame_index % sample_freq == 0;
}

/**
 * @brief AnalysisMethod::run_analysis
 * This is the main loop method for doing an analysis.
 * @return all detections from the performed analysis.
 */
void AnalysisMethod::run() {
    setup_analysis();
    sample_freq = get_setting("SAMPLE_FREQUENCY");
    capture.open(m_source_file);
    if (!capture.isOpened()) {
        return;
    }
    std::vector<DetectionBox> detections;
    num_frames = capture.get(CV_CAP_PROP_FRAME_COUNT);    
    POI* m_POI = new POI();    

    int end_frame = num_frames -1;
    int start_frame = 0;
    // If Interval should be used, use interval frames
    if(analysis_settings->use_interval){
        start_frame = analysis_settings->interval.first;
        capture.set(CV_CAP_PROP_POS_FRAMES, start_frame);
        end_frame = analysis_settings->interval.second;
        num_frames = end_frame - start_frame;
        current_frame_index = start_frame;
    }

    while(!(*aborted) && capture.read(original_frame) &&
          !(analysis_settings->use_interval && (current_frame_index >= end_frame))) {
        // do frame analysis
        if (sample_current_frame() || current_frame_index == end_frame) {
            // Slice frame if bounding box should be used
            if(analysis_settings->use_bounding_box){
                cv::Mat temp (original_frame, analysis_settings->bounding_box);
                analysis_frame = temp;
            }
            else{
                analysis_frame = original_frame;
            }
            // If scaling is needed, i.e if video is high resolution
            // calculate scaling factor and scale frame
            if(!m_scaling_done){
                calculate_scaling_factor();
            }
            if (scaling_needed){
                scale_frame();
            }
            detections = analyse_frame();
            // This if statement handles the sorting of OOIs detected
            // in a frame into the correct POIs.
            if (detections.empty() && detecting) {
                m_POI->set_end_frame(current_frame_index - 1);
                m_analysis.add_interval(m_POI);
                m_POI = new POI();
                detecting = false;
            } else if (!detections.empty()) {
                detecting = true;
                m_POI->add_detections(current_frame_index, detections);
            }
        } else if (!detections.empty()) {
            /* If the current frame is not sampled, the detections from the previously
             * sampled frame should still be valid and should therefore be shown as
             * detections for the current frame as well.
             */
            m_POI->add_detections(current_frame_index, detections);
        }
        if (paused) {
            // TODO do pause stuff
            paused = false;
        }
        // Send progress, update index and release current frame
        emit send_progress(get_progress(start_frame));
        ++current_frame_index;
        original_frame.release();
    }
    if(*aborted){
        capture.release();
        emit analysis_aborted();
        return;
    }else{
        // Makes sure that a POI that stretches to the end of the
        // video gets an end frame.
        if (detecting) {
            m_POI->set_end_frame(current_frame_index);
            m_analysis.add_interval(m_POI);
        }
        capture.release();
        m_analysis.settings = analysis_settings;
        std::string new_path = Utility::add_serial_number(m_save_path + m_ana_name, "");
        m_ana_name = Utility::name_from_path(new_path);
        m_analysis.save_saveable(QString::fromStdString(new_path));
        AnalysisProxy* proxy = new AnalysisProxy(m_analysis, m_analysis.full_path());
        for (auto p : m_analysis.get_intervals()) {
            std::pair<int, int> pair = std::make_pair(p->get_start(), p->get_end());
            proxy->m_slider_interval.push_back(pair);
        }
        emit finished_analysis(proxy);
    }
}

/**
 * @brief AnalysisMethod::get_progress
 * @return Progression of analysis in whole percent.
 */
int AnalysisMethod::get_progress(int start_frame) {
    return ((double)(current_frame_index-start_frame)/(double)num_frames) * 100;

}

/**
 * @brief AnalysisMethod::calculate_scaling_factor
 * This method is used when videos with large resolutions are analysed.
 * To handle the analysis without using too much RAM the frames are
 * resized to fit into a resolution of 1920x1080 before they are analysed.
 * When they are resized, a scaling factor is needed to map detections
 * on a frame to the original resolution of the video. This method does that.
 */
void AnalysisMethod::calculate_scaling_factor() {
    m_scaling_done = true;
    int video_width = analysis_frame.cols;
    int video_height = analysis_frame.rows;
    float height_ratio = float(Constants::FULL_HD_HEIGHT)/float(video_height);
    float width_ratio = float(Constants::FULL_HD_WIDTH)/float(video_width);
    if (height_ratio >= 1 && width_ratio >= 1) return;

    scaling_needed = true;
    //This statement ensures that the original aspect ratio of the video is kept when scaling
    if (width_ratio >= height_ratio) {
        scaling_ratio = height_ratio;
        scaled_width = int(video_width * scaling_ratio);
        scaled_height = Constants::FULL_HD_HEIGHT;
    } else {
        scaling_ratio = width_ratio;
        scaled_width = Constants::FULL_HD_WIDTH;
        scaled_height = int(video_height * scaling_ratio);
    }
}

/**
 * @brief AnalysisMethod::scale_frame
 * This method scales the frames of a video according to the scaling factor.
 */
void AnalysisMethod::scale_frame() {
    cv::Size size(scaled_width,scaled_height);
    cv::Mat dst(size,analysis_frame.type());
    cv::resize(analysis_frame,dst,size); //resize frame
    analysis_frame = dst;
}

/**
 *  Getters and setters
 */
std::string AnalysisMethod::save_path() const {
    return m_save_path;
}

std::string AnalysisMethod::analysis_name() const {
    return m_ana_name;
}

int AnalysisMethod::get_setting(const std::string &var) {
    return analysis_settings->get_setting(var);
}
