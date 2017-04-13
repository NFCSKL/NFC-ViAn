#include "videoproject.h"

VideoProject::VideoProject(Video* v){
    this->video = v;
}

VideoProject::VideoProject(){
    this->video = new Video();
}

Video* VideoProject::get_video(){
    return this->video;
}
/**
 * @brief Video::read
 * @param json
 * Read videoproject parameters from json object.
 */
void VideoProject::read(const QJsonObject& json){
    this->video->read(json);
    QJsonArray json_bookmarks = json["bookmarks"].toArray();
    for(int i = 0; i != json_bookmarks.size(); i++){
        Bookmark* new_bookmark = new Bookmark();
        QJsonObject json_bookmark = json_bookmarks[i].toObject();
        new_bookmark->read(json_bookmark);
        this->bookmarks.push_back(new_bookmark);
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
    for(auto it = bookmarks.begin(); it != bookmarks.end(); it++){
        QJsonObject json_bookmark;
        Bookmark* temp = *it;
        temp->write(json_bookmark);
        json_bookmarks.append(json_bookmark);
    }
    json["bookmarks"] = json_bookmarks;
}

void VideoProject::add_bookmark(Bookmark *bookmark){
    this->bookmarks.push_back(bookmark);
}
