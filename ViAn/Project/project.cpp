#include "project.h"
/**
 * @brief Project::Project
 * @param file_handler
 * @param id
 * @param name
 */
Project::Project(ProjectManager *projet_manager, ID id, std::string name){
    this->project_manager = projet_manager;
    this->name = name;
    this->save_name = name;
    this->id = id;
    this->video_counter = 0;
    this->videos.clear();
    this->changes_made = true;

    // Setting ids to default values, -1 indicating invalid value.
    this->dir = -1;
    this->dir_videos = -1;
    this->dir_bookmarks = -1;
}
/**
 * @brief Project::Project
 */
Project::Project(ProjectManager *projet_manager){
    this->project_manager = projet_manager;
    this->name = "";
    this->save_name = "";
    this->videos.clear();
    this->video_counter = 0;
    // Setting ids to default values, -1 indicating invalid value.
    this->id = -1;
    this->dir = -1;
    this->dir_bookmarks = -1;
    this->dir_videos = -1;

}

/**
 * @brief Project::~Project
 * Clears contents of video map
 */
Project::~Project(){
    for (auto vid_it = this->videos.begin(); vid_it != this->videos.end(); ++vid_it) {
        delete vid_it->second;
    }
    for (auto rep_it = this->reports.begin(); rep_it != this->reports.end(); ++rep_it) {
        delete *rep_it;
    }
}

/**
 * @brief Project::remove_video
 * @param id
 * Remove video from videos and delete its contents.
 */
void Project::remove_video_project(ID id){
    VideoProject* temp = this->videos.at(id);
    delete temp;
    videos.erase(id);
    this->changes_made = true;
}

/**
 * @brief Project::add_video
 * @return Video ID to be used for identifying the video.
 */
ID Project::add_video(Video* vid){
    VideoProject* vid_proj = new VideoProject(vid);
    vid_proj->id  = this->video_counter;
    this->videos.insert(std::make_pair(this->video_counter, vid_proj));
    this->changes_made = true;
    return this->video_counter++;
}

/**
 * @brief Project::add_report
 * @param file_path
 */
void Project::add_report(std::string file_path){
    this->reports.push_back(new Report(file_path));
    this->changes_made = true;
}

/**
 * @brief Project::add_report
 * @param report
 * Required for load, object locally allocated
 */
void Project::add_report(Report* report){
    this->reports.push_back(report);
}

/**
 * @brief Project::add_video
 * @return Video ID to be used for identifying the video.
 */
ID Project::add_video_project(VideoProject* vid_proj){
    vid_proj->id = this->video_counter;
    this->videos.insert(std::make_pair(this->video_counter, vid_proj));
    this->changes_made = true;
    return this->video_counter++;
}
/**
 * @brief Project::delete_artifacts
 * Delete all projects files.
 */
void Project::delete_artifacts(){
    // Delete files in all videoprojects
    for(auto it = videos.begin(); it != videos.end(); it++){
        VideoProject* vp = it->second;
        vp->delete_artifacts();
    }
    // Delete all reports.
    for(auto it = reports.begin(); it != reports.end(); it++){
        Report* temp = *it;
        QFile file (QString::fromStdString(temp->get_file_path()));
        file.remove();
    }
}

/**
 * @brief Project::read
 * @param json
 * Read project parameters from json object.
 */
void Project::read(const QJsonObject& json){
    this->name = json["name"].toString().toStdString();
    this->dir = json["root_dir"].toString().toStdString();
    this->dir_bookmarks = json["bookmark_dir"].toString().toStdString();
    this->dir_videos = json["video_dir"].toString().toStdString();
    this->save_name = this->name;
    // Read videos from json
    QJsonArray json_vid_projs = json["videos"].toArray();
    for (int i = 0; i < json_vid_projs.size(); ++i) {
        QJsonObject json_vid_proj = json_vid_projs[i].toObject();
        VideoProject* v = new VideoProject();
        v->read(json_vid_proj);
        this->add_video_project(v);
    }    
    // Read reports from json
    QJsonArray json_reports = json["reports"].toArray();
    for (int i = 0; i < json_reports.size(); ++i) {
        QJsonObject json_report = json_reports[i].toObject();
        Report* report = new Report();
        report->read(json_report);
        this->add_report(report);
    }
}

/**
 * @brief Project::write
 * @param json
 * Write project parameters to json object.
 */
void Project::write(QJsonObject& json){
    json["name"] = QString::fromStdString(this->name);
    json["root_dir"] =  QString::fromStdString(this->dir);
    json["bookmark_dir"] = QString::fromStdString(this->dir_bookmarks);
    json["video_dir"] = QString::fromStdString(this->dir_videos);
    QJsonArray json_proj;
    // Write Videos to json
    for(auto it = this->videos.begin(); it != this->videos.end(); it++){
        QJsonObject json_vid_proj;
        VideoProject* v = it->second;
        v->write(json_vid_proj);
        json_proj.append(json_vid_proj);
    }
    json["videos"] = json_proj;
    // Write reports to json
    QJsonArray json_reports;
    for(auto it = this->reports.begin(); it != this->reports.end(); it++){
        QJsonObject json_report;
        Report* report = *it;
        report->write(json_report);
        json_reports.append(json_report);
    }
    json["reports"] = json_reports;
}

/**
 * @brief Project::add_analysis
 * @param v_id id of video to add analysis to
 * @param analysis
 */
ID Project::add_analysis(ID v_id, Analysis analysis){
    this->changes_made = true;
    return this->videos.at(v_id)->add_analysis(analysis);
}

/**
 * @brief Project::add_bookmark
 * @param v_id the id of the video
 * @param bookmark
 * Add new bookmark to Videoproj corresponding to id.
 */
ID Project::add_bookmark(ID v_id, Bookmark *bookmark){
    VideoProject* v = this->videos.at(v_id);
    this->changes_made = true;
    return v->add_bookmark(bookmark);
}

/**
 * @brief Project::is_saved
 * @return true if saved
 */
bool Project::is_saved(){
    return !this->changes_made;
}

/**
 * @brief Project::save_project
 * @return sets saved =true
 */
void Project::save_project(){
    save_saveable(dir);
    this->changes_made = false;
}

/**
 * @brief Project::get_videos
 * @return videos&
 */
std::map<ID, VideoProject* > &Project::get_videos(){
    return this->videos;
}

/**
 * @brief Project::get_video
 * @param id
 * @return Returns the video with the specified id.
 */
VideoProject* Project::get_video(ID id) {
    return this->videos[id];
}
