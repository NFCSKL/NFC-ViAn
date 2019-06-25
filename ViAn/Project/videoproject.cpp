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

int VideoProject::get_id() const {
    return id;
}

void VideoProject::set_id(const int& new_id) {
    id = new_id;
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
 * @brief VideoProject::get_analysis
 * @param id of the analysis
 * @return the analysis
 */
BasicAnalysis *VideoProject::get_analysis(const int& id) {
    return m_analyses[id];
}

bool VideoProject::is_saved() {
    bool analyses_saved = std::all_of(m_analyses.begin(), m_analyses.end(),
                                       [](std::map<ID,BasicAnalysis*>::const_reference t){return t.second->is_saved();});
    return !m_unsaved_changes && analyses_saved && m_overlay->is_saved() && m_video->is_saved();
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
    id = json["ID"].toInt();
    QJsonObject json_overlay = json["overlay"].toObject();
    this->m_overlay->read(json_overlay);
    QJsonArray json_analyses = json["analyses"].toArray();

    // Read analyses from json
    for (int j = 0; j < json_analyses.size(); ++j) {
        QJsonObject json_analysis = json_analyses[j].toObject();        
        BasicAnalysis* analysis = nullptr;
        int save_type = json_analysis["analysis_type"].toInt();
        switch(save_type){
        case MOTION_DETECTION:
            analysis = new AnalysisProxy();
            break;
        case TAG:
            analysis = new Tag();
            break;
        case DRAWING_TAG:
            analysis = new Tag();
            break;
        case INTERVAL:
            analysis = new Interval();
            break;
        case SEQUENCE_TAG: {
            Tag* tag = new Tag("Images", SEQUENCE_TAG);
            analysis = tag;
            tag_seq_tag = tag;
            break;
        }
        case OBJECT_DETECTION:
            analysis = new AnalysisProxy();
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
    // Write analyses to json
    QJsonArray json_analyses;
    for(auto it2 = m_analyses.begin(); it2 != m_analyses.end(); it2++){
        QJsonObject json_analysis;
        BasicAnalysis* an = it2->second;
        json_analysis["analysis_type"] = an->get_type();
        an->write(json_analysis);
        json_analyses.append(json_analysis);
    }
    json["analyses"] = json_analyses;
    QJsonObject json_overlay;
    m_overlay->write(json_overlay);
    json["overlay"] = json_overlay;
    m_unsaved_changes = false;
    json["ID"] = id;
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
        ImageSequence* seq = dynamic_cast<ImageSequence*>(m_video);
        if (seq) {
            seq->reset_root_dir(dir);
        }
    }
    for (auto& an : m_analyses) {
        switch (an.second->get_type()) {
        case MOTION_DETECTION:
        case OBJECT_DETECTION:
            dynamic_cast<AnalysisProxy*>(an.second)->reset_root_dir(dir);
            break;
        default:
            break;
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
    analysis->set_vid_proj_id(id);
    if (analysis->get_type() == MOTION_DETECTION ||
            analysis->get_type() == OBJECT_DETECTION) {
        AnalysisProxy* ana_proxy = dynamic_cast<AnalysisProxy*>(analysis);
        m_project->add_analysis(ana_proxy);
    }
    m_unsaved_changes = true;
    return m_ana_id++;
}

void VideoProject::remove_analysis(BasicAnalysis *analysis) {
    if (analysis->get_type() == MOTION_DETECTION ||
            analysis->get_type() == OBJECT_DETECTION) {
        m_project->remove_analysis(dynamic_cast<AnalysisProxy*>(analysis));
    }
    m_analyses.erase(analysis->get_id());
    m_unsaved_changes = true;
    delete analysis;
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

bool VideoProject::is_generated_video() {
    return generated_video;
}

void VideoProject::set_generated_video(bool b) {
    generated_video = b;
}
