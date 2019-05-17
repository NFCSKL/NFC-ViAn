#ifndef YOLOANALYSIS_H
#define YOLOANALYSIS_H

#include "analysismethod.h"

#include "opencv2/dnn/dnn.hpp"

class DetectionBox;

class YoloAnalysis : public AnalysisMethod
{
    // Initialize the parameters
    float confThreshold = 0.5; // Confidence threshold
    float nmsThreshold = 0.4;  // Non-maximum suppression threshold
    int inpWidth = 416;  // Width of network's input image      //320
    int inpHeight = 416; // Height of network's input image
    std::vector<std::string> classes;
    cv::dnn::Net net;
    cv::Mat blob;
public:
    YoloAnalysis(const std::string& source_file, const std::string& save_file, AnalysisSettings* settings);

    void setup_analysis() override;
    std::vector<DetectionBox> analyse_frame() override;

    // Remove the bounding boxes with low confidence using non-maxima suppression
    std::vector<DetectionBox> postprocess(cv::Mat& frame, const std::vector<cv::Mat>& out);

    // Draw the predicted bounding box
    void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame);

    // Get the names of the output layers
    std::vector<cv::String> getOutputsNames(const cv::dnn::Net& net);
};

#endif // YOLOANALYSIS_H
