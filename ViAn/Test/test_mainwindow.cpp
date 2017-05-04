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

void test_mainwindow::test_analysis_queue() {
    MyQTreeWidgetItem *project_in_tree = new MyQTreeWidgetItem(TYPE::PROJECT, "TEST_PROJ", 0);
    mainwindow->ui->project_tree->addTopLevelItem(project_in_tree);
    MyQTreeWidgetItem *video_in_tree = new MyQTreeWidgetItem(TYPE::VIDEO, "seq_01.mp4", 1);
    project_in_tree->addChild(video_in_tree);
    std::string TEST_1 = "TEST_ANALYSIS_1";
    std::string TEST_2 = "TEST_ANALYSIS_2";
    std::string TEST_3 = "TEST_ANALYSIS_3";
    mainwindow->add_analysis_to_tree(ANALYSIS_TYPE::MOTION_DETECTION, QString::fromStdString(TEST_1), video_in_tree);
    mainwindow->add_analysis_to_tree(ANALYSIS_TYPE::MOTION_DETECTION, QString::fromStdString(TEST_2), video_in_tree);
    mainwindow->add_analysis_to_tree(ANALYSIS_TYPE::MOTION_DETECTION, QString::fromStdString(TEST_3), video_in_tree);
    QCOMPARE(mainwindow->current_analysis->id, -1);
    QCOMPARE(mainwindow->current_analysis->name.toStdString(), TEST_1);
    MyQTreeWidgetItem *TEST_ANALYSIS_2 = mainwindow->analysis_queue->dequeue();
    QCOMPARE(TEST_ANALYSIS_2->id, -2);
    QCOMPARE(TEST_ANALYSIS_2->name.toStdString(), TEST_2);
    mainwindow->abort_current_analysis();
    QCOMPARE(mainwindow->current_analysis->id, -1); // gets set to -1 in abort function
    QCOMPARE(mainwindow->current_analysis->name.toStdString(), TEST_3);
    mainwindow->abort_current_analysis();
    delete TEST_ANALYSIS_2;
    delete project_in_tree; // Deletes its children aswell.
}
