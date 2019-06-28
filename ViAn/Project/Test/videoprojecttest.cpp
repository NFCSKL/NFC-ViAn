#include "videoprojecttest.h"

//#include "Filehandler/saveable.h"
#include "Project/Analysis/analysisproxy.h"
#include "Project/Analysis/basicanalysis.h"
#include "Project/bookmark.h"
#include "Project/project.h"
#include "Project/videoproject.h"

#include <QDir>
#include <QJsonObject>
#include <QTest>

#include <memory>

VideoProjectTest::VideoProjectTest(QObject *parent) : QObject(parent)
{

}

/**
 * @brief VideoProjectTest::add_analysis_test
 * Tests if adding analyses to vid_proj, increases
 * size of analyses member
 */
void VideoProjectTest::add_analysis_test(){
    VideoProject* vid_proj = new VideoProject();

    vid_proj->add_analysis(new AnalysisProxy());
    vid_proj->add_analysis(new AnalysisProxy());
    vid_proj->add_analysis(new AnalysisProxy());
    QCOMPARE(vid_proj->m_analyses.size(), unsigned(3));
}

/**
 * @brief VideoProjectTest::delete_analysis
 * Tests if deleting analyses to vid_proj, decreases
 * size of analyses member
 */
void VideoProjectTest::delete_analysis(){
    VideoProject* vid_proj = new VideoProject();

    AnalysisProxy* ana1 = new AnalysisProxy();
    AnalysisProxy* ana2 = new AnalysisProxy();
    AnalysisProxy* ana3 = new AnalysisProxy();

    vid_proj->add_analysis(ana1);
    vid_proj->add_analysis(ana2);
    vid_proj->add_analysis(ana3);
    QCOMPARE(vid_proj->m_analyses.size(), unsigned(3));

    vid_proj->remove_analysis(ana1);
    vid_proj->remove_analysis(ana2);
    vid_proj->remove_analysis(ana3);
    QCOMPARE(vid_proj->m_analyses.size(), unsigned(0));
}

/**
 * @brief VideoProjectTest::read_write_test
 * Tests if written and read video projects to and from Qjson
 * are similar (Doesn't test for complete equality)
 */
void VideoProjectTest::read_write_test(){
    VideoProject* vid_proj = new VideoProject();

    vid_proj->add_analysis(new AnalysisProxy());
    vid_proj->add_analysis(new AnalysisProxy());
    vid_proj->add_analysis(new AnalysisProxy());

    QJsonObject json_vid_proj;
    vid_proj->write(json_vid_proj);

    VideoProject* vid_proj2 = new VideoProject();
    vid_proj2->read(json_vid_proj);

    QCOMPARE(vid_proj->m_ana_id, vid_proj2->m_ana_id);
    QCOMPARE(vid_proj->m_analyses.size(), vid_proj2->m_analyses.size());
}

/**
 * @brief VideoProjectTest::save_load_test
 * Tests if written and read video projects to and from file
 * are similar (Doesn't test for complete equality)
 */
void VideoProjectTest::save_load_delete_test(){
    VideoProject* vid_proj = new VideoProject();

    vid_proj->add_analysis(new AnalysisProxy());
    vid_proj->add_analysis(new AnalysisProxy());
    vid_proj->add_analysis(new AnalysisProxy());

    QDir dir;
    dir.mkpath("C:/TEST/VID_PROJ_TEST/");
    QString file_path("C:/TEST/VID_PROJ_TEST/test");
    vid_proj->save_saveable(file_path, Saveable::JSON);

    VideoProject* vid_proj2 = new VideoProject();
    vid_proj2->load_saveable(file_path);

    QCOMPARE(vid_proj->m_ana_id, vid_proj2->m_ana_id);
    QCOMPARE(vid_proj->m_analyses.size(), vid_proj2->m_analyses.size());

    vid_proj2->delete_saveable();
    dir.rmpath("C:/TEST/VID_PROJ_TEST/");
    QVERIFY(!dir.exists("C:/TEST/VID_PROJ_TEST/"));
}

void VideoProjectTest::save_status_test() {
    std::unique_ptr<VideoProject> video_project(new VideoProject);
    video_project->m_unsaved_changes = false;

    BasicAnalysis* analysis = new BasicAnalysis();
    video_project->add_analysis(analysis);
    QVERIFY(!video_project->is_saved());
    video_project->m_unsaved_changes = false;

    std::stack<int> test_index;
    video_project->set_tree_index(test_index);
    QVERIFY(!video_project->is_saved());
    video_project->m_unsaved_changes = false;

    video_project->set_project(new Project("TEST_NAME","TEST_DIR"));
    QVERIFY(!video_project->is_saved());
    video_project->m_unsaved_changes = false;

    video_project->reset_root_dir("TEST_DIR");
    QVERIFY(!video_project->is_saved());
    video_project->m_unsaved_changes = false;

    video_project->remove_analysis(analysis);
    QVERIFY(!video_project->is_saved());
    video_project->m_unsaved_changes = false;
}
