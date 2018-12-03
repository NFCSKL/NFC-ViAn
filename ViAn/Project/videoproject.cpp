#include "videoproject.h"

#include "bookmark.h"
#include "imagesequence.h"
#include "project.h"
#include "Project/Analysis/analysisproxy.h"
#include "Project/Analysis/basicanalysis.h"
#include "Project/Analysis/tag.h"
#include "Project/Analysis/interval.h"

#include <QDebug>
#include <QJsonArray>

/**
 * @brief VideoProject::VideoProject
 * @param v
 * Set video.
 */
VideoProject::VideoProject(Video* v){
    m_video = v;
}

/**
 * @brief VideoProject::VideoProject
 * Create empty video.
 */
VideoProject::VideoProject(){
    m_video = new Video();
}

VideoProject::~VideoProject(){
    delete m_overlay;
    delete m_video;
}

/**
 * @brief VideoProject::get_video
 * @return video
 * Return associated video.
 */
Video* VideoProject::get_video(){
    return m_video;
}

/**
 * @brief VideoProject::get_overlay
 * @return
 * Returns the overlay.
 */
Overlay* VideoProject::get_overlay() {
    return m_overlay;
}

/**
 * @brief VideoProject::get_bookmarks
 * @return bookmarks
 * Return all bookmarks.
 */
std::map<ID, Bookmark *> VideoProject::get_bookmarks(){
    return m_bookmarks;
}

/**
 * @brief VideoProject::get_analysis
 * @param id of the analysis
 * @return the analysis
 */
BasicAnalysis *VideoProject::get_analysis(const int& id) {
    return m_analyses[id];
}

bool VideoProject::is_saved() {
    bool bookmarks_saved = std::all_of(m_bookmarks.begin(), m_bookmarks.end(),
                                       [](std::map<ID,Bookmark*>::const_reference t){return t.second->is_saved();});
    bool analyses_saved = std::all_of(m_analyses.begin(), m_analyses.end(),
                                       [](std::map<ID,BasicAnalysis*>::const_reference t){return t.second->is_saved();});
    return !m_unsaved_changes && analyses_saved && bookmarks_saved && m_overlay->is_saved() && m_video->is_saved();
}

/**
 * @brief VideoProject::get_analyses
 * @return analyses
 * return all analyses.
 */
std::map<ID, BasicAnalysis*> VideoProject::get_analyses() {
    return m_analyses;
}

/**
 * @brief Video::read
 * @param json
 * Read videoproject parameters from json object.
 */
void VideoProject::read(const QJsonObject& json){
    m_tree_index = json["tree_index"].toString().toStdString();
    Video* vid = new Video();
    vid->read(json);
    if (vid->is_sequence()) {
        ImageSequence* seq = new ImageSequence("");
        seq->read(json);
        m_video = seq;
    } else {
        m_video = vid;
    }


    QJsonArray json_bookmarks = json["bookmarks"].toArray();
    // Read bookmarks from json
    for(int i = 0; i != json_bookmarks.size(); i++){
        QJsonObject json_bookmark = json_bookmarks[i].toObject();
        Bookmark* new_bookmark = new Bookmark();
        new_bookmark->read(json_bookmark);
        add_bookmark(new_bookmark);
    }
    QJsonObject json_overlay = json["overlay"].toObject();
    this->m_overlay->read(json_overlay);
    QJsonArray json_analyses = json["analyses"].toArray();

    // Read analyses from json
    for (int j = 0; j < json_analyses.size(); ++j) {
        QJsonObject json_analysis = json_analyses[j].toObject();        
        BasicAnalysis* analysis = nullptr;
        ANALYSIS_TYPE save_type = (ANALYSIS_TYPE)json_analysis["analysis_type"].toInt();
        switch(save_type){
        case TAG:
            analysis = new Tag();
            break;
        case MOTION_DETECTION:
            analysis = new AnalysisProxy();
            break;
        case DRAWING_TAG:
            analysis = new Tag();
            break;
        case INTERVAL:
            analysis = new Interval();
            break;
        default:
            qWarning("Something went wrong. Undefined analysis");
            return;
        }
        analysis->read(json_analysis);
        add_analysis(analysis);
    }
    m_unsaved_changes = false;
}

/**
 * @brief Video::write
 * @param json
 * Write videoproject parameters to json object.
 */
void VideoProject::write(QJsonObject& json){
    json["tree_index"] = QString::fromStdString(m_tree_index);
    m_video->write(json);
    // Write bookmarks to json
    QJsonArray json_bookmarks;
    for(auto it = m_bookmarks.begin(); it != m_bookmarks.end(); it++){
        QJsonObject json_bookmark;
        Bookmark* temp = it->second;
        temp->write(json_bookmark);
        json_bookmarks.append(json_bookmark);
    }
    json["bookmarks"] = json_bookmarks;
    // Write analyses to json
    QJsonArray json_analyses;
    for(auto it2 = m_analyses.begin(); it2 != m_analyses.end(); it2++){
        QJsonObject json_analysis;
        BasicAnalysis* an = it2->second;
        json_analysis["analysis_type"] = an->get_type(); // Check for type in read
        an->write(json_analysis);
        json_analyses.append(json_analysis);
    }
    json["analyses"] = json_analyses;
    QJsonObject json_overlay;
    m_overlay->write(json_overlay);
    json["overlay"] = json_overlay;
    m_unsaved_changes = false;
}

void VideoProject::set_tree_index(std::stack<int> tree_index) {
    m_tree_index.clear();
    while (!tree_index.empty()) {
        m_tree_index.append(std::to_string(tree_index.top()));
        tree_index.pop();
        m_tree_index.append(":");
    }
    m_unsaved_changes = true;
}

void VideoProject::set_project(Project *proj){
    m_project = proj;
}

void VideoProject::reset_root_dir(const QString &dir) {
    if (m_video->is_sequence()) {
        auto seq = dynamic_cast<ImageSequence*>(m_video);
        if (seq) {
            seq->reset_root_dir(dir);
        }
    }

    for(auto bm : m_bookmarks){
        bm.second->reset_root_dir(dir);
    }
    for(auto& an : m_analyses){
        if(an.second->get_type() == MOTION_DETECTION){ ;
            dynamic_cast<AnalysisProxy*>(an.second)->reset_root_dir(dir);
        }
    }
}

/**
 * @brief VideoProject::add_analysis
 * @param analysis to be added
 * @return id of the analysis
 * Adds analysis to video project.
 */
ID VideoProject::add_analysis(BasicAnalysis *analysis){
    m_analyses.insert(std::make_pair(m_ana_id, analysis));
    analysis->set_id(m_ana_id);
    m_unsaved_changes = true;
    return m_ana_id++;
}

void VideoProject::remove_analysis(BasicAnalysis *analysis) {
    m_analyses.erase(analysis->get_id());
    m_unsaved_changes = true;
    delete analysis;
}

/**
 * @brief VideoProject::add_bookmark
 * @param bookmark
 * Add new bookmark.
 */
ID VideoProject::add_bookmark(Bookmark *bookmark){
    m_bookmarks.insert(std::make_pair(m_bm_cnt, bookmark));
    bookmark->set_id(m_bm_cnt);
    bookmark->set_video_project(this);
    m_unsaved_changes = true;
    return m_bm_cnt++;
}

void VideoProject::remove_bookmark(Bookmark *bookmark) {
    m_bookmarks.erase(bookmark->get_id());
    m_unsaved_changes = true;
    bookmark->remove_exported_image();
    delete bookmark;
}

std::string VideoProject::get_index_path() {
    return m_tree_index;
}

QString VideoProject::get_proj_path() {
    return m_project->get_dir();
}

bool VideoProject::is_current() {
    return current;
}

void VideoProject::set_current(bool value) {
    current = value;
}
