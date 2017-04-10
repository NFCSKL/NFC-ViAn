#include "project.h"
/**
 * @brief Project::Project
 * @param id
 * @param name
 */
Project::Project(ID id, std::string name)
{
    this->name = name;
    this->id = id;
    this->v_id = 0;
    this->videos.clear();
    this->saved = false;
}
/**
 * @brief Project::Project
 */
Project::Project(){
    this->name = "";
    this->id = -1;
    this->id = 0;
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
void Project::remove_video(ID id){
    Video* temp = this->videos.at(id);
    delete temp;
    videos.erase(id);

}

/**
 * @brief Project::add_video
 * @return Video ID to be used for identifying the video
 */
ID Project::add_video(Video* vid){
    vid->id = this->v_id;
    this->videos.insert(std::make_pair(this->v_id, vid));
    return this->v_id++;
}


void Project::read(const QJsonObject& json){
    this->name = json["name"].toString().toStdString();
    QJsonArray json_videos = json["videos"].toArray();
    for (int i = 0; i < json_videos.size(); ++i) {
        QJsonObject json_video = json_videos[i].toObject();
        Video* v = new Video();
        v->read(json_video);
        this->add_video(v);
    }
}

void Project::write(QJsonObject& json){
    QJsonArray json_proj;
    json["name"] = QString::fromStdString(this->name);
    for(auto it = this->videos.begin(); it != this->videos.end(); it++){
        QJsonObject json_video;
        Video* v = it->second;
        v->write(json_video);
        json_proj.append(json_video);
    }
    json["videos"] = json_proj;
}
