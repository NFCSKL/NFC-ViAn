#include "test_report_generator.h"

/**
 * @brief test_report_generator::test_report_generator
 * @param parent
 */
test_report_generator::test_report_generator(QObject *parent) : QObject(parent) {
    file_handler = new FileHandler();
}

/**
 * @brief test_report_generator::test_init
 * This will initalize everything that is needed for testing the report generator.
 */
void test_report_generator::test_init()
{
    std::string dir_path = file_handler->get_work_space().absolutePath().toStdString();
    this->proj1 = file_handler->create_project("TEST_PROJ1", dir_path, dir_path);
    this->proj2 = file_handler->create_project("TEST_PROJ2", dir_path, dir_path);

    std::string video1_path = "seq_01.MP4";
    std::string video2_path = "England4k.mov";
    QString dir_path1 = "C:/TEST_PROJ1/Bookmarks";
    QString bookmark1_path = "england_bild.jpg";
    QString bookmark11_path = "pumparna_bild.jpg";
    QImage image1 = QImage(bookmark1_path);
    QImage image2 = QImage(bookmark11_path);
    Bookmark* bookmark1 = new Bookmark(1000, 1,image1, QString::fromStdString(video1_path), dir_path1, QString::fromStdString("test1"));
    Bookmark* bookmark2 = new Bookmark(2000, 2,image2, QString::fromStdString(video2_path), dir_path1, QString::fromStdString("test11"));


    file_handler->add_video(proj1, video1_path);
    file_handler->add_video(proj1, video2_path);

    proj1->add_bookmark(0,bookmark1);
    proj1->add_bookmark(1,bookmark2);

    file_handler->save_project(proj1);
    file_handler->save_project(proj2);

    report_generator1 = new ReportGenerator(this->proj1, file_handler);
    report_generator2 = new ReportGenerator(this->proj2, file_handler);
}


/**
 * @brief test_report_generator::test_create_and_save_report
 * This test will test that reports are created,saved and stored in the project folder.
 */
void test_report_generator::test_create_and_save_report() {
    //TEST for project 1
    report_generator1->create_report();
    DIR *dir;
    bool document_generated = false;
    struct dirent *ent;
    std::string dir_path = file_handler->get_dir(proj1->dir).absolutePath().toStdString();
    if ((dir = opendir (dir_path.c_str())) != NULL) {
      while ((ent = readdir(dir)) != NULL) {
        std::string test = (std::string)ent->d_name;
        if((test.substr(test.find_last_of(".") + 1) == "docx")){
            document_generated = true;
        }
      }
      closedir (dir);
    }
    QVERIFY(document_generated);

    //TEST for project 2
    document_generated = false;
    report_generator2->create_report();
    dir_path = file_handler->get_dir(proj2->dir).absolutePath().toStdString();
    if ((dir = opendir (dir_path.c_str())) != NULL) {
      while ((ent = readdir(dir)) != NULL) {
        std::string test = (std::string)ent->d_name;
        if((test.substr(test.find_last_of(".") + 1) == "docx")){
            document_generated = true;
        }
      }
      closedir (dir);
    }
    QVERIFY(!document_generated);
}

/**
 * @brief test_report_generator::test_create_list_of_names
 * This test will test that the create list of names method will actually
 * fill in the right amount of objects in the lists based on bookmarks.
 */
void test_report_generator::test_create_list_of_names() {
    report_generator1->all_bookmarks.clear();
    report_generator1->create_list_of_names();
    QCOMPARE(report_generator1->all_bookmarks.size(), unsigned(2));

    report_generator2->all_bookmarks.clear();
    report_generator2->create_list_of_names();
    QCOMPARE(report_generator2->all_bookmarks.size(), unsigned(0));

    file_handler->delete_project(proj1->id);
    file_handler->delete_project(proj2->id);

   // delete file_handler;
    delete report_generator1;
    delete report_generator2;
}
