#include "project.h"

#include "videoproject.h"

#include <QDebug>
#include <QDir>
#include <QFileInfoList>
#include <QJsonArray>
#include <QTemporaryDir>

#include <chrono>
#include <ctime>

/**
 * @brief Project::Project
 * Empty private constructor, used for Project::fromFile
 */
Project::Project(){}

Project* Project::fromFile(const QString &full_path){
    Project* proj = new Project();
    proj->m_temporary = false;
    proj->load_saveable(full_path);
    return proj;
}

/**
 * @brief Project::Project
 * @param name     := name of the project
 * @param dir_path := directory to save project in
 */
Project::Project(const QString& name, const QString& dir_path){
    set_name_and_path(name, dir_path);
    save_project();
}


/**
 * @brief Project::~Project
 * Clears contents of video map
 */
Project::~Project(){
    for (auto video_proj : m_videos) {
        delete video_proj;
    }

    m_videos.clear();
}

/**
 * @brief Project::add_video
 * @return Video ID to be used for identifying the video.
 */
ID Project::add_video_project(VideoProject *vid_proj){
    vid_proj->set_project(this);
    m_videos.push_back(vid_proj);
    m_unsaved_changes = true;
    return m_vid_count++;
}

/**
 * @brief Project::remove_video
 * @param id
 * Remove video from videos and delete its contents.
 * TODO, not used
 */
void Project::remove_video_project(VideoProject* vid_proj){
    auto it = std::find(m_videos.begin(), m_videos.end(), vid_proj);
    if (it == m_videos.end()) return;
    delete *it;
    m_videos.erase(it);
    m_unsaved_changes = true;
}

/**
 * @brief Project::set_saved_status
 * Set unsaved status
 * Should be used by video_project and analysis
 * @param changed
 */
void Project::set_unsaved(const bool& changed) {
    m_unsaved_changes = changed;
}

void Project::set_name_and_path(const QString &name, const QString &path) {
    // TODO: Handle case where tmp directory is empty
    m_name = name;
    if (!path.isEmpty()){
        // Update path for all VideoProjects
        m_dir = path + name + "/";
        for (auto it = begin(m_videos); it != end(m_videos); ++it) {
            (*it)->reset_root_dir(m_dir);
        }
    } else {
        // Attempts to generate a temporary path
        // Use default path on failure
        QString tmp_dir = generate_tmp_directory();
        m_dir = !tmp_dir.isEmpty() ? (tmp_dir + "/") : (Constants::DEFAULT_TEMP_PATH + "/temporary" + QString::number(std::rand()) + "/");
    }
    m_file = m_dir + m_name + ".vian";
    m_dir_bookmarks = m_dir + Constants::BOOKMARK_FOLDER;
}

/**
 * @brief Project::set_temporary
 * Sets whether the project is temporary or not
 * @param is_temporary
 */
void Project::set_temporary(const bool &is_temporary){
    m_temporary = is_temporary;
}

/**
 * @brief Project::is_saved
 * Check if the project has unsaved changes
 * @return m_unsaved_changes
 */
bool Project::is_saved() const {
    bool video_projects_saved = std::all_of(m_videos.begin(), m_videos.end(), [](VideoProject* vp){return vp->is_saved();});
    return !m_unsaved_changes && video_projects_saved;
}

bool Project::is_temporary() const {
    return m_temporary;
}

/**
 * @brief Project::read
 * @param json
 * Read project parameters from json object.
 */
void Project::read(const QJsonObject& json){
    m_name = json["name"].toString();
    m_file = full_path();
    m_dir = m_file.left(m_file.lastIndexOf("/")+1);
    m_dir_bookmarks = m_dir + Constants::BOOKMARK_FOLDER;
    // Read videos from json
    QJsonArray json_vid_projs = json["videos"].toArray();
    for (int i = 0; i < json_vid_projs.size(); ++i) {
        QJsonObject json_vid_proj = json_vid_projs[i].toObject();
        VideoProject* v = new VideoProject();
        v->read(json_vid_proj);
        add_video_project(v);
        v->reset_root_dir(m_dir);
    }    
    m_unsaved_changes = false;
}

/**
 * @brief Project::write
 * @param json
 * Write project parameters to json object.
 */
void Project::write(QJsonObject& json){
    json["name"] = m_name;
    json["root_dir"] =  m_dir;
    QJsonArray json_proj;
    // Write Videos to json
    for(auto it = m_videos.begin(); it != m_videos.end(); it++){
        QJsonObject json_vid_proj;
        (*it)->write(json_vid_proj);
        json_proj.append(json_vid_proj);
    }
    json["videos"] = json_proj;
    m_unsaved_changes = false;
}

/**
 * @brief Project::save_project
 * @return sets saved =true
 */
bool Project::save_project(){
    QDir directory;
    directory.mkpath(m_dir);
    directory.mkpath(m_dir_bookmarks);
    auto time = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(time);
    last_changed = std::ctime(&now_c);
    //last_changed.erase(last_changed.end()-1);       // Remove the "\n"
    last_changed.remove(last_changed.size()-1, 1);
    return save_saveable(m_file);
}

/**
 * @brief Project::copy_directory_files
 * @param from_dir
 * @param to_dir
 * @param coverFileIfExist
 * @return
 * Iterates over from_dir and copies all files and sub-directories to to_dir
 */
bool Project::copy_directory_files(const QString &from_dir, const QString &to_dir, const bool& coverFileIfExist, const std::vector<QString>& exclude_suffix){
    QDir source_dir(from_dir);
    QDir target_dir(to_dir);
    if(!target_dir.exists()){    /* if directory don't exists, build it */
        if(!target_dir.mkdir(target_dir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = source_dir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir()){    /* if it is directory, copy recursively*/
            if(!copy_directory_files(fileInfo.filePath(),
                target_dir.filePath(fileInfo.fileName()),
                coverFileIfExist, exclude_suffix))
                return false;
        }
        else {            /* if coverFileIfExist == true, remove old file first */
            if (std::find(begin(exclude_suffix), end(exclude_suffix), fileInfo.suffix()) != end(exclude_suffix))
                continue;

            if(coverFileIfExist && target_dir.exists(fileInfo.fileName())){
                target_dir.remove(fileInfo.fileName());
            }

            // files copy
            if(!QFile::copy(fileInfo.filePath(),
                target_dir.filePath(fileInfo.fileName()))){
                    return false;
            }
        }
    }
    return true;
}

/**
 * @brief Project::remove_files
 * Removes the project folder and all of its content
 * @return
 */
bool Project::remove_files() {
    QDir path(m_dir);
    bool success{false};

    if (path.exists()) {
        success = path.removeRecursively();
    }
    return success;
}

 /**
 * @brief Project::generate_tmp_directory
 * Tries to generates a temporary directory and returns the path if successful
 */
QString Project::generate_tmp_directory() {
    QTemporaryDir tmp_dir;
    tmp_dir.setAutoRemove(false);
    if(tmp_dir.isValid())
        return tmp_dir.path();
    return "";
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
 *
 * TODO Unused
 */
VideoProject* Project::get_video(const int& v_pos) {
    return m_videos.at(v_pos);
}

QString Project::get_dir() const {
    return m_dir;
}

QString Project::get_name() const {
    return m_name;
}

QString Project::get_file() const {
    return m_file;
}

QString Project::get_last_changed() const {
    return last_changed;
}

void Project::set_name(QString name) {
    m_name = name;
}

void Project::set_dir(QString dir) {
    m_dir = dir;
}

void Project::set_file(QString file) {
    m_file = file;
}
