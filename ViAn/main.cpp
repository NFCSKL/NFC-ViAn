#include "Video/video_player.h"
#include "Test/test_video_player.h"
#include "Test/filehandlertest.h"
#include "Test/test_mainwindow.h"
#include "GUI/mainwindow.h"
#include <QApplication>
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
    w.show();

    bool testing = true;
    if (testing) {
        //For testing video player.
        test_video_player* video_test = new test_video_player();
        QTest::qExec(video_test);
        //For testing filehandler.
        filehandlertest* file_test = new filehandlertest();
        QTest::qExec(file_test);
        //For testing the mainwindow
        test_mainwindow* window_test = new test_mainwindow(&w);
        QTest::qExec(window_test);
    }
    return a.exec();
}
