#include "projecttestsuite.h"

ProjectTestsuite::ProjectTestsuite(QObject *parent) : QObject(parent) {

}
void ProjectTestsuite::add_remove_vid_proj_test()
{
    Project* proj = new Project("TEST_PROJ","C:/");

    VideoProject* vp1 = new VideoProject(new Video("v1"));
    VideoProject* vp2 = new VideoProject(new Video("v2"));
    VideoProject* vp3 = new VideoProject(new Video("v3"));

    int idx1 = proj->add_video_project(vp1);
    int idx2 = proj->add_video_project(vp2);
    int idx3 = proj->add_video_project(vp3);

    QCOMPARE(proj->m_videos.size() , unsigned(3));

    proj->remove_video_project(idx1);
    proj->remove_video_project(idx2);
    proj->remove_video_project(idx3);

    QCOMPARE(proj->m_videos.size() , unsigned(0));
}

void ProjectTestsuite::add_remove_report_test()
{
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

    int idx1 = proj->add_video_project(vp1);
    int idx2 = proj->add_video_project(vp2);
    int idx3 = proj->add_video_project(vp3);

    Report* r1 = new Report("tr_1");
    Report* r2 = new Report("tr_2");
    Report* r3= new Report("tr_3");

    int idx4 = proj->add_report(r1);
    int idx5 = proj->add_report(r2);
    int idx6 = proj->add_report(r3);

    QVERIFY(proj->save_project());

    Project* proj2 = Project::fromFile(proj->full_path());


    QCOMPARE(proj->m_name , proj2->m_name);
    QCOMPARE(proj->m_dir , proj2->m_dir);
    QCOMPARE(proj->m_dir_videos , proj2->m_dir_videos);

    QCOMPARE(proj->m_reports.size(), unsigned(3));
    QCOMPARE(proj->m_videos.size(), unsigned(3));

    proj->remove_video_project(idx1);
    proj->remove_video_project(idx2);
    proj->remove_video_project(idx3);

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
    QVERIFY(!dir.exists(QString::fromStdString(proj->m_dir)));
}
