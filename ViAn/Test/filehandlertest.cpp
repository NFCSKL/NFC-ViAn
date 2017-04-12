#include "filehandlertest.h"

/**
 * @brief filehandlertest::filehandlertest
 * @param parent
 */
FileHandlerTest::FileHandlerTest(QObject *parent) : QObject(parent){
    file_handler = new FileHandler();

}

/**
 * @brief FileHandlerTest::project_test_init
 * creates project for project tests.
 */
void FileHandlerTest::project_test_init()
{
    this->proj = file_handler->create_project("TEST_PROJ");
}

/**
 * @brief FileHandlerTest::project_create_test
 * Test creating and deleting project.
 */
void FileHandlerTest::project_create_delete_test()
{
    Project* proj1 = file_handler->create_project("TEST_PROJ1");
    Project* proj2 = file_handler->create_project("TEST_PROJ2");
    Project* proj3 = file_handler->create_project("TEST_PROJ3");
    QCOMPARE(file_handler->delete_project(proj1->id), 0);
    QCOMPARE(file_handler->delete_project(proj2->id), 0);
    QCOMPARE(file_handler->delete_project(proj3->id), 0);
}

/**
 * @brief FileHandler::project_save_load_test
 * Tests saving and loading projects.
 */
void FileHandlerTest::project_save_load_test(){
    Project* proj1 = file_handler->create_project("TEST_PROJ1");
    Project* proj2 = file_handler->create_project("TEST_PROJ2");
    Project* proj3 = file_handler->create_project("TEST_PROJ3");

    file_handler->save_project(proj1->id);
    file_handler->save_project(proj2->id);
    file_handler->save_project(proj3->id);

    Project* l_proj1 = file_handler->load_project(file_handler->get_dir(proj1->dir).absoluteFilePath("TEST_PROJ1.json").toStdString());
    Project* l_proj2 = file_handler->load_project(file_handler->get_dir(proj2->dir).absoluteFilePath("TEST_PROJ2.json").toStdString());
    Project* l_proj3 = file_handler->load_project(file_handler->get_dir(proj3->dir).absoluteFilePath("TEST_PROJ3.json").toStdString());

    QVERIFY(file_handler->proj_equals(*proj1,*l_proj1));
    QVERIFY(file_handler->proj_equals(*proj2,*l_proj2));
    QVERIFY(file_handler->proj_equals(*proj3,*l_proj3));

    QCOMPARE(file_handler->delete_project(proj1->id), 0);
    QCOMPARE(file_handler->delete_project(proj2->id), 0);
    QCOMPARE(file_handler->delete_project(proj3->id), 0);
}

/**
 * @brief FileHandlerTest::project_add_remove_items
 * Tests adding and removing videos from project, also
 * test information is saved and loaded correctly.
 */
void FileHandlerTest::project_add_remove_items(){
    Project* proj1 = file_handler->create_project("TEST_PROJ1");
    Project* proj2 = file_handler->create_project("TEST_PROJ2");
    Project* proj3 = file_handler->create_project("TEST_PROJ3");

    file_handler->add_video(proj1, "TEST_VID1");
    file_handler->add_video(proj2, "TEST_VID2");
    file_handler->add_video(proj3, "TEST_VID3");

    file_handler->save_project(proj1->id);
    file_handler->save_project(proj2->id);
    file_handler->save_project(proj3->id);


    Project* l_proj1 = file_handler->load_project(file_handler->get_dir(proj1->dir).absoluteFilePath("TEST_PROJ1.json").toStdString());
    Project* l_proj2 = file_handler->load_project(file_handler->get_dir(proj2->dir).absoluteFilePath("TEST_PROJ2.json").toStdString());
    Project* l_proj3 = file_handler->load_project(file_handler->get_dir(proj3->dir).absoluteFilePath("TEST_PROJ3.json").toStdString());

    QVERIFY(file_handler->proj_equals(*proj1,*l_proj1));
    QVERIFY(file_handler->proj_equals(*proj2,*l_proj2));
    QVERIFY(file_handler->proj_equals(*proj3,*l_proj3));

    QCOMPARE(file_handler->delete_project(proj1->id), 0);
    QCOMPARE(file_handler->delete_project(proj2->id), 0);
    QCOMPARE(file_handler->delete_project(proj3->id), 0);
}

/**
 * @brief FileHandlerTest::project_cleanup
 * Deletes project used for project tests.
 */
void FileHandlerTest::project_cleanup()
{
    QCOMPARE(file_handler->delete_project(this->proj->id),0);
}

/**
 * @brief directory_create_test
 * Tests directory creation.
 */
void FileHandlerTest::directory_create_test(){
    QString dirpath = file_handler->get_work_space().absoluteFilePath("TEST_PROJ");
    dir_id = file_handler->create_directory(dirpath);
    QCOMPARE(file_handler->last_error, 1);
    QCOMPARE(file_handler->get_dir(dir_id).absolutePath(), dirpath);

}

/**
 * @brief directory_delete_test
 * Tests deleting directory.
 */
void FileHandlerTest::directory_delete_test(){
    QVERIFY(file_handler->delete_directory(dir_id));
}

/**
 * @brief filehandlertest::fileTest
 *  Tests file creation, deletion, writing and reading.
 */
void FileHandlerTest::file_test_init(){
    QString dirpath = file_handler->get_work_space().absoluteFilePath("TEST_MAP");
    dir_id = file_handler->create_directory(dirpath);
}

/**
 * @brief filehandlertest::file_create_test
 * Tests creating a single file.
 */
void FileHandlerTest::file_create_test(){
    QDir dir = file_handler->get_dir(dir_id);
    this->file_id  = file_handler->create_file(dir.absoluteFilePath("test_file.txt"),dir);
    QCOMPARE(file_handler->get_file(this->file_id), dir.absoluteFilePath("test_file.txt")); // See if file was created in directory
}

/**
 * @brief filehandlertest::file_delete_test
 * Tests deleting a single file.
 */
void FileHandlerTest::file_delete_test(){
    QVERIFY(file_handler->delete_file(this->file_id));
}

/**
 * @brief filehandlertest::file_create_delete_multiple
 * Tests creating multiple files, then deletes these.
 */
void FileHandlerTest::file_create_delete_multiple(){
    // Used directory
    QDir directory = file_handler->get_dir(dir_id);
    // File names
    QString f1 ("filetest1.txt");
    QString f2 ("filetest2.txt");
    QString f3 ("filetest3.txt");
    // Create files
    ID id1 = file_handler->create_file(f1,directory);
    ID id2 = file_handler->create_file(f2,directory);
    ID id3 = file_handler->create_file(f3,directory);
    // Check if all files assigned correct ids
    QCOMPARE(file_handler->get_file(id1), directory.absoluteFilePath(f1));
    QCOMPARE(file_handler->get_file(id2), directory.absoluteFilePath(f2));
    QCOMPARE(file_handler->get_file(id3), directory.absoluteFilePath(f3));
    // Delete files
    QVERIFY(file_handler->delete_file(id1));
    QVERIFY(file_handler->delete_file(id2));
    QVERIFY(file_handler->delete_file(id3));
}

/**
 * @brief FileHandlerTest::file_read_write_init
 * Create test file and test directory.
 */
void FileHandlerTest::file_read_write_init()
{
    this->dir_id = this->file_handler->create_directory(file_handler->get_work_space().absoluteFilePath("TEST_MAP"));
    this->file_id = this->file_handler->create_file("test_file", this->file_handler->get_dir(dir_id));
}

/**
 * @brief filehandlertest::file_read_write_test
 * Test basic read and write.
 */
void FileHandlerTest::file_read_write_test(){
    QString written_text ("test_text");
    QString read_text;
    // Write text to file
    file_handler->write_file(this->file_id, written_text);
    // Read text from file
    file_handler->read_file(this->file_id, read_text, 1);
    QCOMPARE(read_text, written_text);
}

/**
 * @brief filehandlertest::file_read_write_option_test
 * Test read and write options.
 */
void FileHandlerTest::file_read_write_option_test() {
    QString test_text ("test_text");
    QString appended_text("appended_text");
    QString overwrite_text("overwrite_text");

    QString read_text1;                                     // Store read text
    QString read_text2;
    QString read_text3;

    file_handler->write_file(this->file_id, test_text, WRITE_OPTION::OVERWRITE);           // Write test_text to file
    file_handler->read_file(this->file_id, read_text1, 1);                                 // Read test_text from file
    QCOMPARE(read_text1, test_text);                                                       // Confirm
    file_handler->write_file(this->file_id, appended_text, WRITE_OPTION::APPEND);          // Append this text
    file_handler->read_file(this->file_id, read_text2, 1);
    QCOMPARE(read_text2, test_text + appended_text);                                       // Confirm append
    file_handler->write_file(this->file_id, overwrite_text, WRITE_OPTION::OVERWRITE);      // Overwrite previous text
    file_handler->read_file(this->file_id, read_text3, 1);
    QCOMPARE(read_text3, overwrite_text);                                                  // Confirm Overwrite
}

/**
 * @brief filehandlertest::file_read_lines_test
 * Tests reading multiple lines.
 */
void FileHandlerTest::file_read_lines_test(){
    QString read_text;

    QString line1 ("line1");
    QString line2 ("line2");
    QString line3 ("line3");
    QString past_lines ("This should not be read");

    QString written_text  = line1 + "\n" + line2 + "\n" + line3 + "\n" +past_lines;

    file_handler->write_file(this->file_id, written_text, WRITE_OPTION::OVERWRITE); // Write three lines to file, empty

    file_handler->read_file(this->file_id, read_text, 1);                         // Read one line
    QCOMPARE(read_text, line1);
    read_text.clear();
    file_handler->read_file(this->file_id, read_text, 2);                         // Read two lines
    QCOMPARE(read_text, line1 + line2);
    read_text.clear();
    file_handler->read_file(this->file_id, read_text, 3);                         // Read three lines
    QCOMPARE(read_text, line1 + line2 + line3);
    read_text.clear();
}

/**
 * @brief FileHandlerTest::file_read_write_cleanup
 * Delete test file and test directory.
 */
void FileHandlerTest::file_read_write_cleanup()
{
    QVERIFY(file_handler->delete_file(this->file_id));
    QVERIFY(file_handler->delete_directory(this->dir_id));
}



