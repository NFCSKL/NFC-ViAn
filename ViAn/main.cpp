#include <QApplication>
#include <opencv2/opencv.hpp>

#include "Test/test_video_player.h"
#include "Test/test_report_generator.h"
#include "GUI/mainwindow.h"
#include "Project/Analysis/analysismeta.h"


Q_DECLARE_METATYPE(cv::Mat)
/**
 * @brief qMain
 * Constructor
 * @param argc an int variable
 * @param argv a list of chars
 * @return exits the program
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    qRegisterMetaType<AnalysisMeta>();

    w.show();
    return a.exec();
}
