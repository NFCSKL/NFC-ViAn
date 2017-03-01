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


    test_video_player* video_player_test = new test_video_player();
    //filehandlertest* file_handler_test = new filehandlertest();

    QTest::qExec(video_player_test) ;
    //QTest::qExec(file_handler_test) ;


    return a.exec();
}
