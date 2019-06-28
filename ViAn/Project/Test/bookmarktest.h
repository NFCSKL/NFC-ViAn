#ifndef BOOKMARKTEST_H
#define BOOKMARKTEST_H

#include "Project/bookmark.h"

#include <QObject>
#include <QTemporaryDir>

#include <memory>

class Bookmark;

class BookmarkTest : public QObject
{
    Q_OBJECT
    QTemporaryDir m_temp_dir;
    std::unique_ptr<Bookmark> m_bookmark;
private slots:
    void initTestCase();    // Runs before any test function
    void cleanupTestCase(); // Runs after all test functions

    void init();            // Runs before each test case
    void cleanup();         // Runs after each test case

    // Write cases here
    void test_saved_status();
};

#endif // BOOKMARKTEST_H
