#include "projecttestsuite.h"

ProjectTestsuite::ProjectTestsuite(QObject *parent)
{


}

void ProjectTestsuite::init_project_test(){
    m_proj = new Project("TEST_PROJ","C:/", "C:/");
}

void ProjectTestsuite::add_remove_vid_proj_test()
{
    VideoProject* vp1 = new VideoProject(new Video("v1"));
    VideoProject* vp2 = new VideoProject(new Video("v2"));
    VideoProject* vp3 = new VideoProject(new Video("v3"));

    int idx1 = m_proj->add_video_project(vp1);
    int idx2 = m_proj->add_video_project(vp2);
    int idx3 = m_proj->add_video_project(vp3);

    QVERIFY(m_proj->m_reports.size() == 3);

    m_proj->remove_video_project(idx1);
    m_proj->remove_video_project(idx2);
    m_proj->remove_video_project(idx3);

    QVERIFY(m_proj->m_reports.size() == 0);

}

void ProjectTestsuite::add_remove_report_test()
{
    Report* r1 = new Report("tr_1");
    Report* r2 = new Report("tr_2");
    Report* r3= new Report("tr_3");

    int idx1 = m_proj->add_report(r1);
    int idx2 = m_proj->add_report(r2);
    int idx3 = m_proj->add_report(r3);

    QVERIFY(m_proj->m_reports.size() == 3);

    m_proj->remove_report(idx1);
    m_proj->remove_report(idx2);
    m_proj->remove_report(idx3);

    QVERIFY(m_proj->m_reports.size() == 0);
}


void ProjectTestsuite::read_write_test(){
    VideoProject* vp1 = new VideoProject(new Video("v1"));
    VideoProject* vp2 = new VideoProject(new Video("v2"));
    VideoProject* vp3 = new VideoProject(new Video("v3"));

    int idx1 = m_proj->add_video_project(vp1);
    int idx2 = m_proj->add_video_project(vp2);
    int idx3 = m_proj->add_video_project(vp3);

    Report* r1 = new Report("tr_1");
    Report* r2 = new Report("tr_2");
    Report* r3= new Report("tr_3");

    int idx4 = m_proj->add_report(r1);
    int idx5 = m_proj->add_report(r2);
    int idx6 = m_proj->add_report(r3);

    QVERIFY(m_proj->save_project());

    Project* proj2 = Project::fromFile(m_proj->full_path());    ;

    QVERIFY(m_proj->m_name == proj2->m_name);
    QVERIFY(m_proj->m_dir == proj2->m_dir);
    QVERIFY(m_proj->m_dir_videos == proj2->m_dir_videos);

    QVERIFY(m_proj->m_reports.size() == 3);
    QVERIFY(m_proj->m_videos.size() == 3);

    m_proj->remove_video_project(idx1);
    m_proj->remove_video_project(idx2);
    m_proj->remove_video_project(idx3);

    m_proj->remove_report(idx4);
    m_proj->remove_report(idx5);
    m_proj->remove_report(idx6);

}

void ProjectTestsuite::save_load_test(){

}

void ProjectTestsuite::delete_files_test(){

}
