#include "bookmarktest.h"

#include <QTest>

void BookmarkTest::initTestCase(){
    m_temp_dir.setAutoRemove(true);
    QVERIFY(m_temp_dir.isValid());
}

void BookmarkTest::cleanupTestCase(){

}

void BookmarkTest::init() {
    qDebug() << "Creating new bookmark";
    m_bookmark = std::unique_ptr<Bookmark>(new Bookmark);
    m_bookmark->m_unsaved_changes = false; // Force saved status
}

void BookmarkTest::cleanup() {

}

void BookmarkTest::test_saved_status() {
    m_bookmark->reset_root_dir(m_temp_dir.path().toStdString());
    QVERIFY(!m_bookmark->is_saved());
    m_bookmark->m_unsaved_changes = false;

    m_bookmark->set_description("DESCRIPTION_TEST");
    QVERIFY(!m_bookmark->is_saved());
    m_bookmark->m_unsaved_changes = false;

    m_bookmark->set_container("CONTAINER_TEST", 0);
    //m_bookmark->add_container("CONTAINER_TEST", 0); // fix
    QVERIFY(!m_bookmark->is_saved());
    m_bookmark->m_unsaved_changes = false;

    m_bookmark->rename_container("CONTAINER_TEST", "CONTAINER_TEST_OLD");
    QVERIFY(!m_bookmark->is_saved());
    m_bookmark->m_unsaved_changes = false;

//    m_bookmark->remove_container("CONTAINER_TEST_OLD", 0); // fix
//    QVERIFY(!m_bookmark->is_saved());
}
