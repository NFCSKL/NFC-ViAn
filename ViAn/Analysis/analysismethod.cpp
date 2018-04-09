#include <qdebug.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include "Analysis/analysismethod.h"
#include "imagegenerator.h"
AnalysisMethod::AnalysisMethod(const std::string &video_path, const std::string& tmp_save_path, const std::string& save_path)
{
    m_source_file = video_path;
    std::size_t index = video_path.find_last_of('/') + 1;
    std::string vid_name = video_path.substr(index);
    index = vid_name.find_last_of('.');
    vid_name = vid_name.substr(0,index);
    m_ana_name = vid_name + DETECTION_STRING;
    m_tmp_save_path = tmp_save_path;
    m_save_path = save_path;
    add_setting("SAMPLE_FREQUENCY",1, "How often analysis will use frame from video");
}

/**
 * @brief AnalysisMethod::set_include_exclude_area
 * Sets an exlusion frame that will be used to exclude detections in a specific area of each frame.
 * @param points for the polygon that defines the exclusion area.
 */
void AnalysisMethod::set_include_exclude_area(std::vector<cv::Point> points, bool exclude_polygon) {

    if (!capture.isOpened())
        return;

    do_exclusion = true;
    cv::Scalar poly_scalar = cv::Scalar(255);
    cv::Scalar background_scalar = cv::Scalar(0);

    if (exclude_polygon) {
        poly_scalar = cv::Scalar(0);
        background_scalar = cv::Scalar(255);
    }

    cv::Mat img(int(capture.get(cv::CAP_PROP_FRAME_HEIGHT)),int(capture.get(cv::CAP_PROP_FRAME_WIDTH)),CV_8UC1,background_scalar);
    cv::Point* rook_points[1];
    rook_points[0] = &points[0];
    const cv::Point* ppt[1] = {rook_points[0]};
    int npt[1] = {points.size()};
    cv::fillPoly(img, ppt, npt, 1, poly_scalar);
    exclude_frame = img;
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
    if(use_interval){
        start_frame = interval.first;
        capture.set(CV_CAP_PROP_POS_FRAMES, start_frame);
        end_frame = interval.second;
        num_frames = end_frame - start_frame;
        current_frame_index = start_frame;
    }

    while(!(*aborted) && capture.read(original_frame) &&
          !(use_interval && (current_frame_index >= end_frame))) {
        // do frame analysis
        if (sample_current_frame() || current_frame_index == end_frame) {
            // Slice frame if bounding box should be used
            if(use_bounding_box){
                cv::Mat temp (original_frame, bounding_box);
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
        emit finito();
        return;
    }else{
        // Makes sure that a POI that stretches to the end of the
        // video gets an end frame.
        if (detecting) {
            m_POI->set_end_frame(current_frame_index);
            m_analysis.add_interval(m_POI);
        }
        capture.release();
        m_analysis.m_ana_interval = interval;
        m_analysis.bounding_box = bounding_box;
        m_analysis.use_interval = use_interval;
        m_analysis.use_bounding_box = use_bounding_box;
        std::string new_path = Utility::add_serial_number(m_tmp_save_path + m_ana_name, "");
        int index = new_path.find_last_of('/') + 1;
        m_ana_name = new_path.substr(index);

        m_analysis.save_saveable(new_path);

        AnalysisProxy proxy(m_analysis, m_save_path + m_ana_name);
        emit finished_analysis(proxy);
        emit finito();
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
    float height_ratio = float(FULL_HD_HEIGHT)/float(video_height);
    float width_ratio = float(FULL_HD_WIDTH)/float(video_width);
    if (height_ratio >= 1 && width_ratio >= 1) return;

    scaling_needed = true;
    //This statement ensures that the original aspect ratio of the video is kept when scaling
    if (width_ratio >= height_ratio) {
        scaling_ratio = height_ratio;
        scaled_width = int(video_width * scaling_ratio);
        scaled_height = FULL_HD_HEIGHT;
    } else {
        scaling_ratio = width_ratio;
        scaled_width = FULL_HD_WIDTH;
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

cv::Rect AnalysisMethod::get_bounding_box() const {
    return bounding_box;
}

void AnalysisMethod::setBounding_box(const cv::Rect &value) {
    bounding_box = value;
    use_bounding_box = true;
}

std::pair<int, int> AnalysisMethod::get_interval() const {
    return interval;
}

void AnalysisMethod::set_interval(const std::pair<int, int> &value) {
    interval = value;
    use_interval = true;
}

std::string AnalysisMethod::get_descr(const std::string& var)
{
    auto val_pair = m_settings.find(var);
    if(val_pair != m_settings.end())
        return val_pair->first;
    qWarning("No variable \"%s found",var.c_str());
    return "";
}


std::string AnalysisMethod::save_path() const {
    return m_save_path;
}

std::string AnalysisMethod::tmp_save_path() const {
    return m_tmp_save_path;
}

void AnalysisMethod::add_setting(const std::string &var, int value_default, const std::string& descr)
{
    m_settings[var] = value_default;
    m_descriptions[var] = descr;
}

int AnalysisMethod::get_setting(const std::string &var)
{
    auto val_pair = m_settings.find(var);
    if(val_pair != m_settings.end())
        return val_pair->second;
    qWarning("No variable \"%s found",var.c_str());
    return -1;
}

void AnalysisMethod::set_setting(const std::string &var, int value)
{
    m_settings[var] = value;
}

std::vector<std::string> AnalysisMethod::get_var_names()
{
    std::vector<std::string> res;
    for(auto pair : m_settings){
        res.push_back(pair.first);
    }
    return res;
}
