#include "mainwindow.h"
#include <QApplication>
#include "Filehandler/filehandler_testsuite.h"
/**
 * @brief qMain
 * Constructor
 * @param argc an int variable
 * @param argv a list of chars
 * @return exits the program
 */
int main(int argc, char *argv[])
{
    FileHandlerTest* f = new FileHandlerTest();
    QTest::qExec(f);
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
    return 0;
}
