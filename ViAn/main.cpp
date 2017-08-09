#include <QApplication>
#include <opencv2/opencv.hpp>

#include "Test/test_video_player.h"
#include "Test/test_report_generator.h"
#include "GUI/mainwindow.h"
#include "Project/Analysis/analysisproxy.h"
#include "Project/Test/projecttestsuite.h"
#include "Project/Test/videoprojecttest.h"


Q_DECLARE_METATYPE(cv::Mat)
Q_DECLARE_METATYPE(std::string)
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
    /**
     * qRegisterMetaType i
     */
    qRegisterMetaType<std::string>();
    qRegisterMetaType<BasicAnalysis>();
    qRegisterMetaType<AnalysisProxy>();
    bool unit_testing = false;
    if(unit_testing){
        QTest::qExec(new ProjectTestsuite());
        //QTest::qExec(new VideoProjectTest());
    }
    w.show();
    return a.exec();
}
