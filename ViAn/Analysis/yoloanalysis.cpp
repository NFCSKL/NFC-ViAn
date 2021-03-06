#include "yoloanalysis.h"

#include "analysissettings.h"
#include "constants.h"
#include "utility.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <fstream>
#include <iostream>

#include <QDebug>

YoloAnalysis::YoloAnalysis(const std::string& source_file, const std::string& save_file, AnalysisSettings* settings)
    : AnalysisMethod (source_file, save_file, settings) {
    m_analysis.type = OBJECT_DETECTION;

    return;
}

void YoloAnalysis::setup_analysis() {
    // Load names of classes
    Singleton* s = Singleton::get_instance();
    std::string classesFile = s->class_file.toStdString();
    std::ifstream ifs(classesFile.c_str());
    std::string line;
    while (getline(ifs, line)) classes.push_back(line);

    // Give the configuration and weight files for the model
    cv::String modelConfiguration = s->config_file.toStdString();
    cv::String modelWeights = s->weight_file.toStdString();

    // Load the network
    net = cv::dnn::readNetFromDarknet(modelConfiguration, modelWeights);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    inpWidth = get_setting("Network size");
    inpHeight = get_setting("Network size");
    confThreshold = get_setting("Confidence threshold");
    nmsThreshold = get_setting("Nms threshold");
    sample_freq = analysis_settings->frame_rate * (1 / get_setting("Sample frequency (frames/sec)"));
    if (sample_freq == 0) sample_freq++;
    if (sample_freq >= 1000) sample_freq = 1;    // Analyse every frame
}

std::vector<DetectionBox> YoloAnalysis::analyse_frame() {
    std::vector<DetectionBox> OOIs;

    // Create a 4D blob from a frame.
    cv::dnn::blobFromImage(analysis_frame, blob, 1 / 255.0, cv::Size(inpWidth, inpHeight), cv::Scalar(0, 0, 0), true, false);

    //Sets the input to the network
    net.setInput(blob);

    // Runs the forward pass to get output of the output layers
    std::vector<cv::Mat> outs;
    net.forward(outs, getOutputsNames(net));

    // Remove the bounding boxes with low confidence
    OOIs = postprocess(analysis_frame, outs);

    // Write the frame with the detection boxes
    cv::Mat detectedFrame;
    analysis_frame.convertTo(detectedFrame, CV_8U);

    return OOIs;
}

// Remove the bounding boxes with low confidence using non-maxima suppression
std::vector<DetectionBox> YoloAnalysis::postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs) {
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;
    std::vector<DetectionBox> detection_boxes;

    for (size_t i = 0; i < outs.size(); ++i) {
        // Scan through all the bounding boxes output from the network and keep only the
        // ones with high confidence scores. Assign the box's class label as the class
        // with the highest score for the box.

        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols) {
            cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            cv::Point classIdPoint;
            double confidence;
            // Get the value and location of the maximum score
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > confThreshold) {
                int centerX = static_cast<int>((data[0] * frame.cols));
                int centerY = static_cast<int>((data[1] * frame.rows));
                int width = static_cast<int>((data[2] * frame.cols));
                int height = static_cast<int>((data[3] * frame.rows));
                int left = centerX - width / 2;
                int top = centerY - height / 2;

                classIds.push_back(classIdPoint.x);
                confidences.push_back(static_cast<float>(confidence));

                int x_diff = left + width - frame.cols;
                if (x_diff > 0) width = width - x_diff;

                int y_diff = top + height - frame.rows;
                if (y_diff > 0) height = height - y_diff;

                cv::Rect box = cv::Rect(left, top, width, height);
                boxes.push_back(box);
            }
        }
    }

    // Perform non maximum suppression to eliminate redundant overlapping boxes with
    // lower confidences
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
    for (size_t i = 0; i < indices.size(); ++i) {
        int idx = indices[i];
        DetectionBox box = DetectionBox(boxes[idx]);
        box.set_confidence(confidences[idx]);
        if (!classes.empty()) {
            box.set_class_name(classes[classIds[idx]]);
        } else {
            box.set_class_name("Not found");
        }

        if(analysis_settings->use_bounding_box) {
            cv::Rect slice_rect = analysis_settings->bounding_box;
            cv::Rect rect_to_original (box.get_rect().tl()+slice_rect.tl(), slice_rect.tl()+box.get_rect().br());
            box.update_rect(rect_to_original);
        }
        if(scaling_needed) box.update_rect(Utility::scale_rect(box.get_rect(), scaling_ratio, cv::Point(0,0)));
        detection_boxes.push_back(box);
    }
    return detection_boxes;
}

// Get the names of the output layers
std::vector<cv::String> YoloAnalysis::getOutputsNames(const cv::dnn::Net& net) {
    static std::vector<cv::String> names;
    if (names.empty()) {
        //Get the indices of the output layers, i.e. the layers with unconnected outputs
        std::vector<int> outLayers = net.getUnconnectedOutLayers();

        //get the names of all the layers in the network
        std::vector<cv::String> layersNames = net.getLayerNames();

        // Get the names of the output layers in names
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i) {
            names[i] = layersNames[outLayers[i] - 1];
        }
    }
    return names;
}
