#include "filehandlertest.h"

/**
 * @brief filehandlertest::filehandlertest
 * @param parent
 */
filehandlertest::filehandlertest(QObject *parent) : QObject(parent){
    file_handler = new FileHandler();
}
/**
 * @brief FileHandlerTest::projectHandlingTest
 * Tests project creation and deletion.
 * TODO : tests project write to file and read from file
 */
void filehandlertest::project_handling_test(){
    FileHandler* fh = file_handler;
    Project* proj =  fh->create_project("TEST_PROJ");
    std::string v1 = "video1.txt"; // names for testfiles
    std::string v2 = "video2.txt";
    std::string v3 = "video3.txt";
    ID vid1 = fh->create_file(v1, proj->files->dir);  // create testfiles
    ID vid2 = fh->create_file(v2, proj->files->dir);
    ID vid3 = fh->create_file(v3, proj->files->dir);
    fh->add_video(proj, fh->get_file(vid1)); //add tesfiles as videos to project.
    fh->add_video(proj, fh->get_file(vid2));
    fh->add_video(proj, fh->get_file(vid3));

    fh->save_project(proj);
    //check file contents
    Project* proj2 = fh->load_project("TEST_PROJ", fh->work_space + "TEST_PROJ");
    QVERIFY(fh->proj_equals(*proj2,*proj));

    //check project contentss
    fh->delete_file(vid1);
    fh->delete_file(vid2);
    fh->delete_file(vid3);
    QCOMPARE(fh->delete_project(proj), 0);
}

void filehandlertest::project_create_test()
{
    proj = file_handler->create_project("Test_project");

}

/**
 * @brief directory_create_test
 * Tests directory creation.
 */
void filehandlertest::directory_create_test(){
    std::string dirpath = file_handler->work_space + std::string("TEST_PROJ");
    dir_id = file_handler->create_directory(dirpath);
    QCOMPARE(file_handler->last_error, 0);
    QCOMPARE(file_handler->get_dir(id), dirpath);

}
/**
 * @brief directory_delete_test
 * Tests deleting directory.
 */
void filehandlertest::directory_delete_test(){
    QCOMPARE(file_handler->delete_directory(dir_id), 0);
}

/**
 * @brief filehandlertest::fileTest
 *  Tests file creation, deletion, writing and reading.
 */
void filehandlertest::file_test_init(){
    std::string dirpath = fh->work_space + "TEST_MAP";
    dir_id = create_directory(dirpath);
}
/**
 * @brief filehandlertest::file_create_test
 * Tests creating a single file.
 */
void filehandlertest::file_create_test(){
    std::string filename = "filetest.txt";
    file_id  = fh->create_file(filename,dir_id);
    QCOMPARE(file_handler->get_file(file_id), file_handler->get_dir(dir_id) + "/" + filename); // See if file was created in directory
}
/**
 * @brief filehandlertest::file_delete_test
 * Tests deleting a single file.
 */
void filehandlertest::file_delete_test(){
    QCOMPARE(file_handler->delete_file(file_id), 0);
}
/**
 * @brief filehandlertest::file_create_delete_multiple
 * Tests creating multiple files, then deletes these.
 */
void filehandlertest::file_create_delete_multiple(){
    // Used directory
    std::string directory = file_handler->get_dir(dir_id);
    // File names
    std::string f1 = "filetest1.txt";
    std::string f2 = "filetest2.txt";
    std::string f3 = "filetest3.txt";
    // Create files
    ID id1 = file_handler->create_file(f1,dir_id);
    ID id2 = file_handler->create_file(f2,dir_id);
    ID id3 = file_handler->create_file(f3,dir_id);
    // Check if all files assigned correct ids
    QCOMPARE(file_handler->get_file(id1), directory + "/" + f1);
    QCOMPARE(file_handler->get_file(id2), directory + "/" + f2);
    QCOMPARE(file_handler->get_file(id3), directory + "/" + f3);
    // Delete files
    QCOMPARE(file_handler->delete_file(id1), 0);
    QCOMPARE(file_handler->delete_file(id2), 0);
    QCOMPARE(file_handler->delete_file(id3), 0);
}
/**
 * @brief filehandlertest::file_read_write_test
 * Test basic read and write.
 */
void filehandlertest::file_read_write_test(){
    std::string written_text ("test_text");
    std::string read_text;
    ID file_id = file_handler->file_id;
    // Write text to file
    file_handler->write_file(file_id, test_text);
    // Read text from file
    file_handler->read_file(file_id, read_text, 1);
    QCOMPARE(read_text, written_text);
}
/**
 * @brief filehandlertest::file_read_write_option_test
 * Test read and write options.
 */
void filehandlertest::file_read_write_option_test() {
    std::string test_text ("test_text");
    std::string appended_text("appended_text");
    std::string overwrite_text("overwrite_text");
    std::string read_text;                          // Store read text

    file_handler->write_file(file_id, test_text);   // Write test_text to file
    file_handler->read_file(file_id, read_text, 1); // Read test_text from file
    QCOMPARE(read_text, test_text);                 // Confirm
    file_handler->write_file(file_id, appended_text, WRITE_OPTION::APPEND);     // Append this text
    file_handler->read_file(file_id, read_text, 1);
    QCOMPARE(read_text, test_text + appended_text);                             // Confirm append

    file_handler->write_file(file_id, overwrite_text, WRITE_OPTION::OVERWRITE); // Overwrite previous text
    file_handler->read_file(file_id, read_text, 1);
    QCOMPARE(read_text, overwrite_text);                                        // Confirm Overwrite
}
/**
 * @brief filehandlertest::file_read_lines_test
 * Tests reading multiple lines.
 */
void filehandlertest::file_read_lines_test(){
    std::string read_text;
    std::string line1 = "line1\n";
    std::string line1 = "line2\n";
    std::string line1 = "line3\n";
    std::string past_lines = "This should not be read";
    file_handler->write_file(file_id, test_lines, WRITE_OPTION::OVERWRITE); // Write three lines to file
    file_handler->read_file(file_id, read_text, 1);                         // Read one line
    QCOMPARE(read_text, line1);
    file_handler->read_file(file_id, read_text, 2);                         // Read two lines
    QCOMPARE(read_text, line1 + line2);
    file_handler->read_file(file_id, read_text, 3);                         // Read three lines
    QCOMPARE(read_text, line1 + line2 + line3);
    file_handler->read_file(file_id, read_text, 4);                         // Read four lines
    QCOMPARE(read_text, line1 + line2 + line3);

}



