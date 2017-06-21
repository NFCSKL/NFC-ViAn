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

    vid_proj->add_analysis(new AnalysisMeta());
    vid_proj->add_analysis(new AnalysisMeta());
    vid_proj->add_analysis(new AnalysisMeta());
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

    int id1  = vid_proj->add_analysis(new AnalysisMeta());
    int id2  = vid_proj->add_analysis(new AnalysisMeta());
    int id3  = vid_proj->add_analysis(new AnalysisMeta());
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

    vid_proj->add_analysis(new AnalysisMeta());
    vid_proj->add_analysis(new AnalysisMeta());
    vid_proj->add_analysis(new AnalysisMeta());

    vid_proj->add_bookmark(new Bookmark());
    vid_proj->add_bookmark(new Bookmark());
    vid_proj->add_bookmark(new Bookmark());

    QJsonObject json_vid_proj;
    vid_proj->write(json_vid_proj);

    VideoProject* vid_proj2 = new VideoProject();
    vid_proj2->read(json_vid_proj);

    QCOMPARE(vid_proj->m_ana_cnt, vid_proj2->m_ana_cnt);
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

    vid_proj->add_analysis(new AnalysisMeta());
    vid_proj->add_analysis(new AnalysisMeta());
    vid_proj->add_analysis(new AnalysisMeta());

    vid_proj->add_bookmark(new Bookmark());
    vid_proj->add_bookmark(new Bookmark());
    vid_proj->add_bookmark(new Bookmark());

    QDir dir;
    dir.mkpath("C:/TEST/VID_PROJ_TEST/");
    std::string file_path("C:/TEST/VID_PROJ_TEST/test");
    vid_proj->save_saveable(file_path, Saveable::JSON);

    VideoProject* vid_proj2 = new VideoProject();
    vid_proj2->load_saveable(file_path + ".json");

    QCOMPARE(vid_proj->m_ana_cnt, vid_proj2->m_ana_cnt);
    QCOMPARE(vid_proj->m_bm_cnt, vid_proj2->m_bm_cnt);
    QCOMPARE(vid_proj->m_analyses.size(), vid_proj2->m_analyses.size());
    QCOMPARE(vid_proj->m_bookmarks.size(), vid_proj2->m_bookmarks.size());

    vid_proj2->delete_saveable();
    dir.rmpath("C:/TEST/VID_PROJ_TEST/");
    QVERIFY(!dir.exists("C:/TEST/VID_PROJ_TEST/"));
}
