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
    return overlay;
}

/**
 * @brief VideoProject::get_bookmarks
 * @return bookmarks
 * Return all bookmarks.
 */
std::map<ID, Bookmark *> VideoProject::get_bookmarks(){
    return this->bookmarks;
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
        QJsonObject json_bookmark = json_bookmarks[i].toObject();
        Bookmark* new_bookmark = new Bookmark();
        new_bookmark->read(json_bookmark);
        add_bookmark(new_bookmark);
    }
    QJsonObject json_overlay = json["overlay"].toObject();
    this->overlay->read(json_overlay);
}

/**
 * @brief Video::write
 * @param json
 * Write videoproject parameters to json object.
 */
void VideoProject::write(QJsonObject& json){
    this->video->write(json);
    QJsonArray json_bookmarks;
    // Needs to delete bookmarks before exporting the new ones.
    delete_artifacts();
    for(auto it = bookmarks.begin(); it != bookmarks.end(); it++){
        QJsonObject json_bookmark;
        Bookmark* temp = it->second;
        temp->write(json_bookmark);
        json_bookmarks.append(json_bookmark);
    }
    json["bookmarks"] = json_bookmarks;
    QJsonObject json_overlay;
    this->overlay->write(json_overlay);
    json["overlay"] = json_overlay;
}

/**
 * @brief VideoProject::add_bookmark
 * @param bookmark
 * Add new bookmark.
 */
ID VideoProject::add_bookmark(Bookmark *bookmark){
    this->bookmarks.insert(std::make_pair(id_bookmark, bookmark));
    return id_bookmark++;
}

/**
 * @brief VideoProject::delete_artifacts
 * Delete bookmark files.
 */
void VideoProject::delete_artifacts(){
    for(auto it = bookmarks.begin(); it != bookmarks.end(); it++){
        Bookmark* temp = it->second;
        temp->remove_exported_image();
    }
}

