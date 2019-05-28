#include "yoloanalysis.h"

#include "analysissettings.h"
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

    // Open a video file or an image file or a camera stream.
    std::string str, outputFile;
    cv::VideoCapture cap;
    cv::VideoWriter video;
    cv::Mat frame, blob;
    str = "D:/Testdata/filmer/FHP-P112.avi";
    int input_type = 2;      // 1 = image, 2 = video, 3 = webcam

    try {
        if (input_type == 1) {
            qDebug() << "image";
            // Open the image file
            std::ifstream ifile(str);
            if (!ifile) throw("error");
            cap.open(str);
            str.replace(str.end() - 4, str.end(), "_yolo_out_cpp.jpg");
            outputFile = str;
        } else if (input_type == 2) {
            qDebug() << "video";
            // Open the video file
            std::ifstream ifile(str);
            if (!ifile) throw("error");
            cap.open(str);
            str.replace(str.end() - 4, str.end(), "_yolo_out_cpp.avi");
            outputFile = str;
        } else if (input_type == 3) {
            cap.open(0);
        }

    }
    catch (...) {
        std::cout << "Could not open the input image/video stream" << std::endl;
        return;
    }

    // Get the video writer initialized to save the output video
    if (input_type == 2) {
        video.open(outputFile, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 28, cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)));
    }

    // Create a window
    static const std::string kWinName = "Deep learning object detection in OpenCV";
    namedWindow(kWinName, cv::WINDOW_NORMAL);

    // Process frames.
    while (cv::waitKey(1) < 0) {
        // get frame from the video
        cap >> frame;

        // Stop the program if reached end of video
        if (frame.empty()) {
            std::cout << "Done processing !!!" << std::endl;
            std::cout << "Output file is stored as " << outputFile << std::endl;
            cv::waitKey(3000);
            break;
        }
        // Create a 4D blob from a frame.
        cv::dnn::blobFromImage(frame, blob, 1 / 255.0, cv::Size(inpWidth, inpHeight), cv::Scalar(0, 0, 0), true, false);

        //Sets the input to the network
        net.setInput(blob);

        // Runs the forward pass to get output of the output layers
        std::vector<cv::Mat> outs;
        net.forward(outs, getOutputsNames(net));

        // Remove the bounding boxes with low confidence
        postprocess(frame, outs);

        // Put efficiency information. The function getPerfProfile returns the overall time for inference(t) and the timings for each of the layers(in layersTimes)
        std::vector<double> layersTimes;
        double freq = cv::getTickFrequency() / 1000;
        double t = net.getPerfProfile(layersTimes) / freq;
        std::string label = cv::format("Inference time for a frame : %.2f ms", t);
        putText(frame, label, cv::Point(0, 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));

        // Write the frame with the detection boxes
        cv::Mat detectedFrame;
        frame.convertTo(detectedFrame, CV_8U);

        if (input_type == 1) {
            imwrite(outputFile, detectedFrame);
        } else video.write(detectedFrame);

        imshow(kWinName, frame);
    }

    cap.release();
    if (input_type == 2) {
        video.release();
    }
}

void YoloAnalysis::setup_analysis() {
    // Load names of classes
    std::string classesFile = "C:/Users/Student/Desktop/yolo in opencv/yolo3/coco.names";
    std::ifstream ifs(classesFile.c_str());
    std::string line;
    while (getline(ifs, line)) classes.push_back(line);

    // Give the configuration and weight files for the model
    cv::String modelConfiguration = "C:/Users/Student/Desktop/yolo in opencv/yolo3/yolov3.cfg";
    cv::String modelWeights = "C:/Users/Student/Desktop/yolo in opencv/yolo3/yolov3.weights";

    // Load the network
    net = cv::dnn::readNetFromDarknet(modelConfiguration, modelWeights);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    inpWidth = get_setting("Network size");
    inpHeight = get_setting("Network size");
    confThreshold = get_setting("Confidence threshold");
    nmsThreshold = get_setting("Nms threshold");
    sample_freq = analysis_settings->frame_rate * get_setting("Sample frequency (frames/sec)");
    if (sample_freq == 0) sample_freq++;
    if (sample_freq >= 100) sample_freq = 1;    // Analyse every frame
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
                boxes.push_back(cv::Rect(left, top, width, height));
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


// TODO Not currently used
// Draw the predicted bounding box
void YoloAnalysis::drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame) {
    //Draw a rectangle displaying the bounding box
    rectangle(frame, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(255, 178, 50), 3);

    //Get the label for the class name and its confidence
    std::string label = cv::format("%.2f", conf);
    if (!classes.empty()) {
        CV_Assert(classId < static_cast<int>(classes.size()));
        label = classes[classId] + ":" + label;
    }

    //Display the label at the top of the bounding box
    int baseLine;
    cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    top = std::max(top, labelSize.height);
    rectangle(frame, cv::Point(left, top - round(1.5*labelSize.height)), cv::Point(left + round(1.5*labelSize.width), top + baseLine), cv::Scalar(255, 255, 255), cv::FILLED);
    putText(frame, label, cv::Point(left, top), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 0), 1);
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
