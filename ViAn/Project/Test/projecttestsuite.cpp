#include "projecttestsuite.h"

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

void ProjectTestsuite::add_remove_report_test(){
    Project* proj = new Project("TEST_PROJ","C:/");

    Report* r1 = new Report("tr_1");
    Report* r2 = new Report("tr_2");
    Report* r3= new Report("tr_3");

    int idx1 = proj->add_report(r1);
    int idx2 = proj->add_report(r2);
    int idx3 = proj->add_report(r3);

    QCOMPARE(proj->m_reports.size() , unsigned(3));

    proj->remove_report(idx1);
    proj->remove_report(idx2);
    proj->remove_report(idx3);

    QCOMPARE(proj->m_reports.size(), unsigned(0));
}


void ProjectTestsuite::save_load_test(){
    Project* proj = new Project("TEST_PROJ","C:/");

    VideoProject* vp1 = new VideoProject(new Video("v1"));
    VideoProject* vp2 = new VideoProject(new Video("v2"));
    VideoProject* vp3 = new VideoProject(new Video("v3"));

    proj->add_video_project(vp1);
    proj->add_video_project(vp2);
    proj->add_video_project(vp3);

    Report* r1 = new Report("tr_1");
    Report* r2 = new Report("tr_2");
    Report* r3= new Report("tr_3");

    int idx4 = proj->add_report(r1);
    int idx5 = proj->add_report(r2);
    int idx6 = proj->add_report(r3);

    QVERIFY(proj->save_project());

    Project* proj2 = Project::fromFile(proj->full_path());


    QCOMPARE(proj->m_name , proj2->m_name);
    QCOMPARE(proj->m_tmp_dir , proj2->m_tmp_dir);

    QCOMPARE(proj->m_reports.size(), unsigned(3));
    QCOMPARE(proj->m_videos.size(), unsigned(3));

    proj->remove_video_project(vp1);
    proj->remove_video_project(vp2);
    proj->remove_video_project(vp3);

    proj->remove_report(idx4);
    proj->remove_report(idx5);
    proj->remove_report(idx6);
}

void ProjectTestsuite::delete_files_test(){
    Project* proj = new Project("TEST_PROJ","C:/");

    VideoProject* vp1 = new VideoProject(new Video("v1"));
    VideoProject* vp2 = new VideoProject(new Video("v2"));
    VideoProject* vp3 = new VideoProject(new Video("v3"));

    proj->add_video_project(vp1);
    proj->add_video_project(vp2);
    proj->add_video_project(vp3);

    Report* r1 = new Report("tr_1");
    Report* r2 = new Report("tr_2");
    Report* r3= new Report("tr_3");

    proj->add_report(r1);
    proj->add_report(r2);
    proj->add_report(r3);

    proj->save_project();
    proj->delete_artifacts();
    QDir dir;
    QVERIFY(!dir.exists(QString::fromStdString(proj->m_tmp_dir)));
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

    std::string project_path = directory.path().toStdString();
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

    // VIDEOPROJECT/REPORT
    // Verify that adding a report updates status
    Report* report = new Report();
    int report_id = project->add_report(report);
    QVERIFY(!project->is_saved());
    project->save_project();

    // Verify that removing a report updates status
    project->remove_report(report_id);
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
