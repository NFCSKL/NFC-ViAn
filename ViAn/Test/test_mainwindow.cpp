#include "test_mainwindow.h"
/**
 * @brief test_mainwindow::test_mainwindow
 * @param mainwindow the window to be tested
 * @param parent
 */
test_mainwindow::test_mainwindow(MainWindow *mainwindow, QObject *parent) : QObject(parent) {
    this->mainwindow = mainwindow;
}
/**
 * @brief test_mainwindow::test_set_status_bar
 */
test_mainwindow::test_set_status_bar() {
    mainwindow->set_status_bar("test");
    QCOMPARE(mainwindow->statusBar()->currentMessage(), QString::fromStdString("test"));
}
