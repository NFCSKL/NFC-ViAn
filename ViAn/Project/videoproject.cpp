#include "videoproject.h"

/**
 * @brief VideoProject::VideoProject
 * @param v
 * Set video.
 */
VideoProject::VideoProject(Video* v){
    this->video = v;    
}

/**
 * @brief VideoProject::VideoProject
 * Create empty video.
 */
VideoProject::VideoProject(){
    this->video = new Video();
}

/**
 * @brief VideoProject::get_video
 * @return video
 * Return associated video.
 */
Video* VideoProject::get_video(){
    return this->video;
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
    return this->m_bookmarks;
}

/**
 * @brief VideoProject::get_analysis
 * @param id of the analysis
 * @return the analysis
 */
AnalysisMeta* VideoProject::get_analysis(const int& id) {
    return m_analyses[id];
}

/**
 * @brief VideoProject::remove_analysis
 * @param id of the analysis
 */
void VideoProject::delete_analysis(const int& id) {
    AnalysisMeta* am = m_analyses.at(id);
    m_analyses.erase(id);
    am->delete_saveable();
    delete am;
}

/**
 * @brief VideoProject::delete_bookmark
 * @param id
 */
void VideoProject::delete_bookmark(const int &id)
{
    Bookmark* bm = m_bookmarks.at(id);
    m_bookmarks.erase(id);
    bm->remove_exported_image();
    delete bm;
}

/**
 * @brief VideoProject::get_analyses
 * @return analyses
 * return all analyses.
 */
std::map<ID, AnalysisMeta *> VideoProject::get_analyses() {
    return m_analyses;
}

/**
 * @brief Video::read
 * @param json
 * Read videoproject parameters from json object.
 */
void VideoProject::read(const QJsonObject& json){
    this->video->read(json);
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
        AnalysisMeta* analysis = new AnalysisMeta();
        analysis->read(json_analysis);
        add_analysis(analysis);
    }
}

/**
 * @brief Video::write
 * @param json
 * Write videoproject parameters to json object.
 */
void VideoProject::write(QJsonObject& json){
    this->video->write(json);
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
        AnalysisMeta* an = it2->second;
        an->write(json_analysis);
        json_analyses.append(json_analysis);
    }
    json["analyses"] = json_analyses;

    QJsonObject json_overlay;
    this->m_overlay->write(json_overlay);
    json["overlay"] = json_overlay;
}

/**
 * @brief VideoProject::add_bookmark
 * @param bookmark
 * Add new bookmark.
 */
ID VideoProject::add_bookmark(Bookmark *bookmark){
    this->m_bookmarks.insert(std::make_pair(m_bm_cnt, bookmark));
    return m_bm_cnt++;
}

/**
 * @brief VideoProject::add_analysis
 * @param analysis to be added
 * @return id of the analysis
 * Adds analysis to video project.
 */
ID VideoProject::add_analysis(AnalysisMeta* analysis){
    this->m_analyses.insert(std::make_pair(this->m_ana_cnt, analysis));
    return this->m_ana_cnt++;
}

/**
 * @brief VideoProject::delete_artifacts
 * Delete bookmark files.
 */
void VideoProject::delete_artifacts(){
    for(auto it = m_bookmarks.begin(); it != m_bookmarks.end(); it++){
        Bookmark* temp = it->second;
        temp->remove_exported_image();
    }
    for(auto it2 = m_analyses.begin(); it2 != m_analyses.end(); it2++){
        AnalysisMeta* temp = it2->second;
        temp->delete_saveable();

    }
}
