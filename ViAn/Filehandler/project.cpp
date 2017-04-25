#include "project.h"
/**
 * @brief Project::Project
 * @param id
 * @param name
 */
Project::Project(ID id, std::string name)
{
    this->name = name;
    this->save_name = name;
    this->id = id;
    this->dir = -1;
    this->dir_videos = -1;
    this->bookmark_dir = -1;
    this->v_id = 0;
    this->videos.clear();
    this->saved = false;
}
/**
 * @brief Project::Project
 */
Project::Project(){
    this->name = "";
    this->save_name = "";
    this->id = -1;
    this->id = 0;
    this->dir = -1;
    this->bookmark_dir = -1;
    this->dir_videos = -1;
    this->videos.clear();
}

/**
 * @brief Project::~Project
 * Clears contents of video map
 */
Project::~Project(){
    for (auto vidIt = this->videos.begin(); vidIt != this->videos.end(); ++vidIt) {
        delete vidIt->second;
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

}

/**
 * @brief Project::add_video
 * @return Video ID to be used for identifying the video.
 */
ID Project::add_video(Video* vid){
    vid->id = this->v_id;
    this->videos.insert(std::make_pair(this->v_id, new VideoProject(vid)));
    return this->v_id++;
}

/**
 * @brief Project::add_video
 * @return Video ID to be used for identifying the video.
 */
ID Project::add_video_project(VideoProject* vid_proj){
    vid_proj->get_video()->id = this->v_id;
    this->videos.insert(std::make_pair(this->v_id, vid_proj));
    return this->v_id++;
}
/**
 * @brief Project::delete_artifacts
 * Delete all projects files.
 */
void Project::delete_artifacts(){
    for(auto it = videos.begin(); it != videos.end(); it++){
        VideoProject* vp = it->second;
        vp->delete_artifacts();
    }
}

/**
 * @brief Project::read
 * @param json
 * Read project parameters from json object.
 */
void Project::read(const QJsonObject& json){
    this->name = json["name"].toString().toStdString();
    this->save_name = this->name;
    QJsonArray json_vid_projs = json["videos"].toArray();
    for (int i = 0; i < json_vid_projs.size(); ++i) {
        QJsonObject json_vid_proj = json_vid_projs[i].toObject();
        VideoProject* v = new VideoProject();
        v->read(json_vid_proj);
        this->add_video_project(v);
    }
}

/**
 * @brief Project::write
 * @param json
 * Write project parameters to json object.
 */
void Project::write(QJsonObject& json){
    QJsonArray json_proj;
    json["name"] = QString::fromStdString(this->name);
    for(auto it = this->videos.begin(); it != this->videos.end(); it++){
        QJsonObject json_vid_proj;
        VideoProject* v = it->second;
        v->write(json_vid_proj);
        json_proj.append(json_vid_proj);
    }
    json["videos"] = json_proj;
}

void Project::add_analysis(Analysis an){
    this->videos.at(id)->add_analysis(an);
}

/**
 * @brief Project::add_bookmark
 * @param bookmark
 * Add new bookmark to Videoproj corresponding to id.
 */
void Project::add_bookmark(ID id, Bookmark *bookmark){
    VideoProject* v = this->videos.at(id);
    v->add_bookmark(bookmark);
}

