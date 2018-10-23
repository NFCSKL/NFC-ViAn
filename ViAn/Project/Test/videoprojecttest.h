#ifndef VIDEOPROJECTTEST_H
#define VIDEOPROJECTTEST_H

#include <QObject>

class VideoProjectTest : public QObject
{
    Q_OBJECT
public:
    explicit VideoProjectTest(QObject *parent = nullptr);

private slots:
    void add_analysis_test();
    void add_bookmark_test();
    void read_write_test();
    void delete_analysis();
    void delete_bookmark();
    void save_load_delete_test();
    void save_status_test();
};

#endif // VIDEOPROJECTTEST_H
