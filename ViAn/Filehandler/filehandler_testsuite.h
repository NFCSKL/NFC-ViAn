#ifndef FILEHANDLER_TESTSUITE_H
#define FILEHANDLER_TESTSUITE_H
#include "filehandler.h"
#include "project.h"
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
