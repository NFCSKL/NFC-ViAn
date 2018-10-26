#ifndef PROJECTTESTSUITE_H
#define PROJECTTESTSUITE_H

#include <QObject>

/**
 * @brief The ProjectTestsuite class
 * This class tests the usage of Projects public interface.
 */
class ProjectTestsuite : public QObject
{
    Q_OBJECT
public:
    explicit ProjectTestsuite(QObject *parent = nullptr);
private slots:
    void add_remove_vid_proj_test();
    void save_load_test();
    void delete_files_test();
    void save_status_test();
};

#endif // PROJECTTESTSUITE_H
