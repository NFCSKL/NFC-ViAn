#include "project.h"
#include <QDebug>
/**
 * @brief Project::Project
 * Empty private constructor, used for Project::fromFile
 */
Project::Project()
{

}
//TODO Fix all these function names
Project* Project::fromFile(const std::string &full_path)
{
    Project* proj = new Project();
    proj->load_saveable(full_path);
    return proj;
}

/**
 * @brief Project::Project
 * @param name     := name of the project
 * @param dir_path := directory to save project in
 * @param vid_path := path for videos folder
 */
Project::Project(const std::string& name, const std::string& dir_path){
    m_name = name;    
    m_dir = dir_path + "/" + name + "/";
    m_dir_bookmarks = m_dir + "/Bookmarks/";
    m_dir_videos = dir_path;
    m_file = m_dir + name;
}


/**
 * @brief Project::~Project
 * Clears contents of video map
 */
Project::~Project(){
    for (auto vid_it = m_videos.begin(); vid_it != m_videos.end(); ++vid_it) {
        delete *vid_it;
    }
    for (auto rep_it = m_reports.begin(); rep_it != m_reports.end(); ++rep_it) {
        delete rep_it->second;
    }
}

/**
 * @brief Project::add_video
 * @return Video ID to be used for identifying the video.
 */
void Project::add_video_project(VideoProject *vid_proj){
    vid_proj->set_project(this);
    m_videos.push_back(vid_proj);
}

/**
 * @brief Project::remove_video
 * @param id
 * Remove video from videos and delete its contents.
 */
void Project::remove_video_project(VideoProject* vid_proj){
    auto it = std::find(m_videos.begin(), m_videos.end(), vid_proj);
    if (it == m_videos.end()) return;
    m_videos.erase(it);
}


/**
 * @brief Project::add_report
 * @param report
 * Required for load, object locally allocated
 */
ID Project::add_report(Report *report){
    m_reports.insert(std::make_pair(m_rp_count,report));   
    return m_rp_count++;
}

/**
 * @brief Project::remove_report
 * @param id
 */
void Project::remove_report(const int &id)
{
    Report* temp = m_reports.at(id);
    delete temp;
    m_reports.erase(id);
}


/**
 * @brief Project::delete_artifacts
 * Delete all projects files.
 */
void Project::delete_artifacts(){
    // Delete files in all videoprojects
    for (auto it = m_videos.begin(); it != m_videos.end(); it++) {
        (*it)->delete_artifacts();
    }

    // Delete all reports.
    for(auto it = m_reports.begin(); it != m_reports.end(); it++){
        Report* temp = (*it).second;
        QFile file (QString::fromStdString(temp->get_file_path()));
        file.remove();
    }
    // Delete directories
    delete_saveable();
    QDir directory;
    QString q_dir = QString::fromStdString(m_dir);
    QString q_dir_bookmarks = QString::fromStdString(m_dir_bookmarks);
    directory.rmdir(q_dir_bookmarks);
    directory.rmdir(q_dir);
}

/**
 * @brief Project::read
 * @param json
 * Read project parameters from json object.
 */
void Project::read(const QJsonObject& json){
    m_name = json["name"].toString().toStdString();
    m_dir = json["root_dir"].toString().toStdString();
    m_dir_bookmarks = json["bookmark_dir"].toString().toStdString();
    m_dir_videos = json["video_dir"].toString().toStdString();
    m_file = m_dir + m_name;
    // Read videos from json
    QJsonArray json_vid_projs = json["videos"].toArray();
    for (int i = 0; i < json_vid_projs.size(); ++i) {
        QJsonObject json_vid_proj = json_vid_projs[i].toObject();
        VideoProject* v = new VideoProject();
        v->read(json_vid_proj);
        add_video_project(v);
    }    
    // Read reports from json
    QJsonArray json_reports = json["reports"].toArray();
    for (int i = 0; i < json_reports.size(); ++i) {
        QJsonObject json_report = json_reports[i].toObject();
        Report* report = new Report();
        report->read(json_report);
        add_report(report);
    }
}

/**
 * @brief Project::write
 * @param json
 * Write project parameters to json object.
 */
void Project::write(QJsonObject& json){
    json["name"] = QString::fromStdString(m_name);
    json["root_dir"] =  QString::fromStdString(m_dir);
    json["bookmark_dir"] = QString::fromStdString(m_dir_bookmarks);
    json["video_dir"] = QString::fromStdString(m_dir_videos);
    json["full_path"] = QString::fromStdString(m_file);
    QJsonArray json_proj;
    // Write Videos to json
    for(auto it = m_videos.begin(); it != m_videos.end(); it++){
        QJsonObject json_vid_proj;
        (*it)->write(json_vid_proj);
        json_proj.append(json_vid_proj);
    }
    json["videos"] = json_proj;
    // Write reports to json
    QJsonArray json_reports;
    for(auto it = m_reports.begin(); it != m_reports.end(); it++){
        QJsonObject json_report;
        Report* report = it->second;
        report->write(json_report);
        json_reports.append(json_report);
    }
    json["reports"] = json_reports;
}

/**
 * @brief Project::save_project
 * @return sets saved =true
 */
bool Project::save_project(){
    QDir directory;
    directory.mkpath(QString::fromStdString(m_dir));
    directory.mkpath(QString::fromStdString(m_dir_bookmarks));
    return save_saveable(m_file);
}

/**
 * @brief Project::get_videos
 * @return videos&
 */
std::vector<VideoProject*> &Project::get_videos(){
    return m_videos;
}

/**
 * @brief Project::get_video
 * @param id
 * @return Returns the video with the specified id.
 */
VideoProject* Project::get_video(const int& v_pos) {
    return m_videos.at(v_pos);
}

/**
 * @brief Project::Project
 */
std::string Project::getDir_bookmarks() const
{
    return m_dir_bookmarks;
}

std::string Project::getDir_videos() const
{
    return m_dir_videos;
}

std::string Project::getDir() const {
    return m_dir;
}

std::string Project::getName() const
{
    return m_name;
}
