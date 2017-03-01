#include "Video/video_player.h"
#include "Test/test_video_player.h"
#include "GUI/mainwindow.h"
#include <QApplication>
#include <QTest>
#include "Filehandler/filehandler.h"
#include "Test/filehandlertest.h"
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

    //For testing video player.
    //test_video_player* test = new test_video_player();
    //filehandlertest* test = new filehandlertest();
    //QTest::qExec(test) ;
    return a.exec();
}
