
#include <QApplication>
#include "GUI/mainwindow.h"
#include "Video/video_player.h"
#include "Test/test_video_player.h"
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
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

    //For testing video player.
    //test_video_player* test = new test_video_player();
    //QTest::qExec(test) ;

    //For testing file handler.
    filehandlertest* test = new filehandlertest();
    QTest::qExec(test) ;
    return 0;
 //   return a.exec();
}
