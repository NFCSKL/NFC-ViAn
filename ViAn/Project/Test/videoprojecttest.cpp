#include "videoprojecttest.h"

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
 * @brief VideoProjectTest::add_bookmark_test
 * Tests if adding bookmark to vid_proj, increases
 * size of bookmark member
 */
void VideoProjectTest::add_bookmark_test(){
    VideoProject* vid_proj = new VideoProject();

    vid_proj->add_bookmark(new Bookmark());
    vid_proj->add_bookmark(new Bookmark());
    vid_proj->add_bookmark(new Bookmark());
    QCOMPARE(vid_proj->m_bookmarks.size(), unsigned(3));
}

/**
 * @brief VideoProjectTest::delete_analysis
 * Tests if deleting analyses to vid_proj, decreases
 * size of analyses member
 */
void VideoProjectTest::delete_analysis(){
    VideoProject* vid_proj = new VideoProject();

    int id1  = vid_proj->add_analysis(new AnalysisProxy());
    int id2  = vid_proj->add_analysis(new AnalysisProxy());
    int id3  = vid_proj->add_analysis(new AnalysisProxy());
    QCOMPARE(vid_proj->m_analyses.size(), unsigned(3));

    vid_proj->delete_analysis(id1);
    vid_proj->delete_analysis(id2);
    vid_proj->delete_analysis(id3);
    QCOMPARE(vid_proj->m_analyses.size(), unsigned(0));
}

/**
 * @brief VideoProjectTest::delete_bookmark
 * Tests if deleting bookmark to vid_proj, decreases
 * size of bookmark member
 */
void VideoProjectTest::delete_bookmark(){
    VideoProject* vid_proj = new VideoProject();

    int id1  = vid_proj->add_bookmark(new Bookmark());
    int id2  = vid_proj->add_bookmark(new Bookmark());
    int id3  = vid_proj->add_bookmark(new Bookmark());
    QCOMPARE(vid_proj->m_bookmarks.size(), unsigned(3));

    vid_proj->delete_bookmark(id1);
    vid_proj->delete_bookmark(id2);
    vid_proj->delete_bookmark(id3);
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

    vid_proj->add_bookmark(new Bookmark());
    vid_proj->add_bookmark(new Bookmark());
    vid_proj->add_bookmark(new Bookmark());

    QJsonObject json_vid_proj;
    vid_proj->write(json_vid_proj);

    VideoProject* vid_proj2 = new VideoProject();
    vid_proj2->read(json_vid_proj);

    QCOMPARE(vid_proj->m_ana_id, vid_proj2->m_ana_id);
    QCOMPARE(vid_proj->m_bm_cnt, vid_proj2->m_bm_cnt);
    QCOMPARE(vid_proj->m_analyses.size(), vid_proj2->m_analyses.size());
    QCOMPARE(vid_proj->m_bookmarks.size(), vid_proj2->m_bookmarks.size());
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

    vid_proj->add_bookmark(new Bookmark());
    vid_proj->add_bookmark(new Bookmark());
    vid_proj->add_bookmark(new Bookmark());

    QDir dir;
    dir.mkpath("C:/TEST/VID_PROJ_TEST/");
    std::string file_path("C:/TEST/VID_PROJ_TEST/test");
    vid_proj->save_saveable(file_path, Saveable::JSON);

    VideoProject* vid_proj2 = new VideoProject();
    vid_proj2->load_saveable(file_path);

    QCOMPARE(vid_proj->m_ana_id, vid_proj2->m_ana_id);
    QCOMPARE(vid_proj->m_bm_cnt, vid_proj2->m_bm_cnt);
    QCOMPARE(vid_proj->m_analyses.size(), vid_proj2->m_analyses.size());
    QCOMPARE(vid_proj->m_bookmarks.size(), vid_proj2->m_bookmarks.size());

    vid_proj2->delete_saveable();
    dir.rmpath("C:/TEST/VID_PROJ_TEST/");
    QVERIFY(!dir.exists("C:/TEST/VID_PROJ_TEST/"));
}

void VideoProjectTest::save_status_test() {
    std::unique_ptr<VideoProject> video_project(new VideoProject);
    video_project->m_unsaved_changes = false;

    BasicAnalysis* analysis = new BasicAnalysis();
    int analysis_id = video_project->add_analysis(analysis);
    QVERIFY(!video_project->is_saved());
    video_project->m_unsaved_changes = false;

    Bookmark* bookmark = new Bookmark();
    int bookmark_id = video_project->add_bookmark(bookmark);
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

    video_project->delete_analysis(analysis_id);
    QVERIFY(!video_project->is_saved());
    video_project->m_unsaved_changes = false;

    video_project->delete_bookmark(bookmark_id);
    QVERIFY(!video_project->is_saved());
    video_project->m_unsaved_changes = false;

    video_project->delete_artifacts();
    QVERIFY(!video_project->is_saved());
    video_project->m_unsaved_changes = false;

    video_project->remove_from_project();
    QVERIFY(!video_project->is_saved());
    video_project->m_unsaved_changes = false;
}
