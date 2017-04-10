#ifndef FILEHANDLERTEST_H
#define FILEHANDLERTEST_H
#include "Filehandler/filehandler.h"
#include "Filehandler/project.h"
#include <QTest>
#include <QObject>


class FileHandlerTest : public QObject
{
    Q_OBJECT

public:
    explicit FileHandlerTest(QObject *parent = 0);
private slots:
    void directory_test();
    void file_test();
    void project_handling_test();

};

#endif // FILEHANDLER_TESTSUITE_H
