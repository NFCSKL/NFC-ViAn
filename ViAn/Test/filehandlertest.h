#ifndef FILEHANDLER_H
#define FILEHANDLERTEST_H
#include "../Filehandler/filehandler.h"
#include "../Filehandler/project.h"
#include <QtTest>


class FileHandlerTest : public QObject
{
    Q_OBJECT

public:
    FileHandlerTest();
private slots:
    void directoryTest();
    void fileTest();
    void projectHandlingTest();

};

#endif // FILEHANDLER_TESTSUITE_H
