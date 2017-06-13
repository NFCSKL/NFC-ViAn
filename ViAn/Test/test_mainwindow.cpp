#include "test_mainwindow.h"
#include <string.h>
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
void test_mainwindow::test_set_status_bar() {
    std::string s = "test";
    mainwindow->set_status_bar(s);
    QCOMPARE(mainwindow->statusBar()->currentMessage(), QString::fromStdString(s));
}
