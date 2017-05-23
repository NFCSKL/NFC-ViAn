#ifndef TEST_MAINWINDOW_H
#define TEST_MAINWINDOW_H

#include <QObject>
#include <QTest>
#include "GUI/mainwindow.h"


class test_mainwindow : public QObject
{
    Q_OBJECT
public:
    explicit test_mainwindow(MainWindow *mainwindow, QObject *parent = 0);
private slots:
    void test_set_status_bar();
    void test_analysis_queue();
private:
    MainWindow *mainwindow;
};

#endif // TEST_MAINWINDOW_H
