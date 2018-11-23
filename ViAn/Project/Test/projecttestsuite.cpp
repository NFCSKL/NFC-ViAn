#include "projecttestsuite.h"

//#include <QJsonObject>
////vian
#include "Project/Analysis/basicanalysis.h"
#include "Project/bookmark.h"
#include "Project/project.h"
#include "Project/video.h"
#include "Project/videoproject.h"

////QT
#include <QDir>
#include <QTemporaryDir>
#include <QTest>

#include <memory>

ProjectTestsuite::ProjectTestsuite(QObject *parent) : QObject(parent) {
}

void ProjectTestsuite::add_remove_vid_proj_test(){
    Project* proj = new Project("TEST_PROJ","C:/");

    VideoProject* vp1 = new VideoProject(new Video("v1"));
    VideoProject* vp2 = new VideoProject(new Video("v2"));
    VideoProject* vp3 = new VideoProject(new Video("v3"));

    proj->add_video_project(vp1);
    proj->add_video_project(vp2);
    proj->add_video_project(vp3);

    QCOMPARE(proj->m_videos.size() , unsigned(3));

    proj->remove_video_project(vp1);
    proj->remove_video_project(vp2);
    proj->remove_video_project(vp3);

    QCOMPARE(proj->m_videos.size() , unsigned(0));

    delete proj;
}


void ProjectTestsuite::save_load_test(){
    // TODO Fix

    Project* proj = new Project("TEST_PROJ","C:/");

    VideoProject* vp1 = new VideoProject(new Video("v1"));
    VideoProject* vp2 = new VideoProject(new Video("v2"));
    VideoProject* vp3 = new VideoProject(new Video("v3"));

    proj->add_video_project(vp1);
    proj->add_video_project(vp2);
    proj->add_video_project(vp3);

    QVERIFY(proj->save_project());

    Project* proj2 = Project::fromFile(proj->get_file());


    QCOMPARE(proj->m_name , proj2->m_name);

    QCOMPARE(proj->m_videos.size(), unsigned(3));

    proj->remove_video_project(vp1);
    proj->remove_video_project(vp2);
    proj->remove_video_project(vp3);

    delete proj; // Need more
}

void ProjectTestsuite::delete_files_test(){
    Project* proj = new Project("TEST_PROJ","C:/");

    VideoProject* vp1 = new VideoProject(new Video("v1"));
    VideoProject* vp2 = new VideoProject(new Video("v2"));
    VideoProject* vp3 = new VideoProject(new Video("v3"));

    proj->add_video_project(vp1);
    proj->add_video_project(vp2);
    proj->add_video_project(vp3);

    proj->save_project();
    proj->remove_files();
    QDir dir;
}

/**
 * @brief ProjectTestsuite::save_status_test
 * Tests if changes made to the project structure properly
 * updates the save status of the project instance.
 * This is important since this status is used to prompt
 * the user to save before closing a project.
 */
void ProjectTestsuite::save_status_test(){
    // Create temporary project directory
    QTemporaryDir directory;
    directory.setAutoRemove(true);
    QVERIFY(directory.isValid());

    QString project_path = directory.path();
    std::unique_ptr<Project> project(new Project("testproject", project_path));
    QVERIFY(!project->is_saved());

    // Verify save updates status correctly
    project->save_project();
    QVERIFY(project->is_saved());

    // VIDEOPROJECT
    // Verify that adding a video project updates status
    VideoProject* video_project = new VideoProject(new Video("v1"));
    project->add_video_project(video_project);
    QVERIFY(!project->is_saved());
    project->save_project();

    // VIDEOPROJECT/BOOKMARK
    // Verify that modifying a video project updates status
    // Verify that adding a bookmark updates status
    Bookmark* bookmark = new Bookmark();
    int bookmark_id = video_project->add_bookmark(bookmark);
    QVERIFY(!project->is_saved());
    project->save_project();

    // Verify that removing a bookmark updates status
    video_project->delete_bookmark(bookmark_id);
    QVERIFY(!project->is_saved());
    project->save_project();

    // VIDEOPROJECT/ANALYSIS
    // Verify that adding an analysis updates status
    BasicAnalysis* analysis = new BasicAnalysis();
    int analysis_id = video_project->add_analysis(analysis);
    QVERIFY(!project->is_saved());
    project->save_project();

    // Verify that removing an analysis updates status
    video_project->delete_analysis(analysis_id);
    QVERIFY(!project->is_saved());
    project->save_project();

    // Verify that removing a video project updates status
    project->remove_video_project(video_project);
    QVERIFY(!project->is_saved());
    project->save_project();
}
