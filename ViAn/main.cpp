
#include "GUI/mainwindow.h"
#include "GUI/recentprojectdialog.h"
#include "Project/Analysis/analysisproxy.h"
#include "Project/Analysis/basicanalysis.h"
#include "Project/Test/bookmarktest.h"
#include "Project/Test/projecttestsuite.h"
#include "Project/Test/videoprojecttest.h"

#include "opencv2/core/core.hpp"

#include <QApplication>
#include <QSplashScreen>
#include <QTest>
#include <QTimer>

#include <windows.h>

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
    QPixmap pixmap(":/Splashscreen.png");
    QSplashScreen splash(pixmap);
    splash.setWindowFlags(splash.windowFlags() & Qt::WindowStaysOnTopHint);
    splash.show();
    a.processEvents();
    QTimer::singleShot(1000, &splash, &QSplashScreen::close); // Timer
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
        QTest::qExec(new BookmarkTest());
        QTest::qExec(new VideoProjectTest());
    }
    w.show();
    w.rp_dialog->exec();
    return a.exec();
}
