#include "project.h"
#include <QDebug>
/**
 * @brief Project::Project
 * Empty private constructor, used for Project::fromFile
 */
Project::Project(){
}

//TODO Fix all these function names
Project* Project::fromFile(const std::string &full_path){
    Project* proj = new Project();
    proj->load_saveable(full_path);
    // ensure changes to paths are saved
    proj->save_saveable(full_path);
    if(proj->tmp_dir.isValid()){
        proj->m_tmp_dir = proj->tmp_dir.path().toStdString() + "/" + proj->m_name + "/";
        proj->m_tmp_file = proj->m_tmp_dir + proj->m_name + ".vian";
        proj->save_project();
    } else return nullptr;
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
    if (dir_path == "default") {
        is_default_proj = true;
    } else {
        m_dir = dir_path + "/" + name + "/";
        m_file = m_dir + name + ".vian";
        m_unsaved_changes = true;
    }

    if(tmp_dir.isValid()){
        m_tmp_dir = tmp_dir.path().toStdString() + "/" + name + "/";
        m_dir_bookmarks = m_tmp_dir + "Bookmarks/";
        m_tmp_file = m_tmp_dir + name + ".vian";  // full_path();
        save_project();
        tmp_dir_valid = true;
    } else qWarning() << "Something went wrong while creating the temporary folder.";
}


/**
 * @brief Project::~Project
 * Clears contents of video map
 */
Project::~Project(){
    m_videos.clear();
    m_reports.clear();
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
    m_videos.erase(it);
    m_unsaved_changes = true;
}


/**
 * @brief Project::add_report
 * @param report
 * Required for load, object locally allocated
 */
ID Project::add_report(Report *report){
    m_reports.insert(std::make_pair(m_rp_count,report));   
    m_unsaved_changes = true;
    return m_rp_count++;
}

/**
 * @brief Project::remove_report
 * @param id
 */
void Project::remove_report(const int &id) {
    Report* temp = m_reports.at(id);
    m_reports.erase(id);
    m_unsaved_changes = true;
}


/**
 * @brief Project::delete_artifacts
 * Delete all projects files.
 */
void Project::delete_artifacts(){
    QDir dir(QString::fromStdString(m_dir));
    // Delete directory and all of its contents
    // Including subdirectories and their contents.
    dir.removeRecursively();
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

/**
 * @brief Project::is_saved
 * Check if the project has unsaved changes
 * @return m_unsaved_changes
 */
bool Project::is_saved() const {
    bool video_projects_saved = std::all_of(m_videos.begin(), m_videos.end(), [](VideoProject* vp){return vp->is_saved();});
    return !m_unsaved_changes && video_projects_saved;
}

/**
 * @brief Project::read
 * @param json
 * Read project parameters from json object.
 */
void Project::read(const QJsonObject& json){
    m_name = json["name"].toString().toStdString();
    m_file = full_path();
    std::string tmp = full_path();
    m_dir = tmp.substr(0,tmp.find_last_of("/")+1);
    m_dir_bookmarks = m_dir + "Bookmarks/";
    // Read videos from json
    QJsonArray json_vid_projs = json["videos"].toArray();
    for (int i = 0; i < json_vid_projs.size(); ++i) {
        QJsonObject json_vid_proj = json_vid_projs[i].toObject();
        VideoProject* v = new VideoProject();
        v->read(json_vid_proj);
        add_video_project(v);
        v->reset_root_dir(m_dir);
    }    
    // Read reports from json
    QJsonArray json_reports = json["reports"].toArray();
    for (int i = 0; i < json_reports.size(); ++i) {
        QJsonObject json_report = json_reports[i].toObject();
        Report* report = new Report();
        report->read(json_report);
        add_report(report);
        report->reset_root_dir(m_dir);
    }
    m_unsaved_changes = false;
}

/**
 * @brief Project::write
 * @param json
 * Write project parameters to json object.
 */
void Project::write(QJsonObject& json){
    json["name"] = QString::fromStdString(m_name);
    json["root_dir"] =  QString::fromStdString(m_tmp_dir);
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
    m_unsaved_changes = false;
}

/**
 * @brief Project::save_project
 * @return sets saved =true
 */
bool Project::save_project(){
    QDir directory;
    directory.mkpath(QString::fromStdString(m_tmp_dir));
    directory.mkpath(QString::fromStdString(m_dir_bookmarks));
    return save_saveable(m_tmp_file);
}

/**
 * @brief Project::save_project
 * @return sets saved =true
 */
bool Project::move_project_from_tmp(){
    return copy_directory_files(QString::fromStdString(m_tmp_dir), QString::fromStdString(m_dir), true);
}

/**
 * @brief Project::copy_directory_files
 * @param from_dir
 * @param to_dir
 * @param coverFileIfExist
 * @return
 * Iterates over from_dir and copies all files and sub-directories to to_dir
 */
bool Project::copy_directory_files(const QString &from_dir, const QString &to_dir, bool coverFileIfExist){
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
                coverFileIfExist))
                return false;
        }
        else {            /* if coverFileIfExist == true, remove old file first */
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
std::string Project::getDir_bookmarks() const {
    return m_dir_bookmarks;
}

std::string Project::get_dir() const {
    return m_dir;
}

std::string Project::get_tmp_dir() const {
    return m_tmp_dir;
}

std::string Project::get_name() const {
    return m_name;
}

std::string Project::get_file() const {
    return m_file;
}

void Project::set_name(std::string name) {
    m_name = name;
}

void Project::set_dir(std::string dir) {
    m_dir = dir;
}

void Project::set_file(std::string file) {
    m_file = file;
}

void Project::update_tmp(string name) {
    QFile::rename(QString::fromStdString(m_tmp_file), QString::fromStdString(m_tmp_dir + name + ".vian"));
    QDir dir = QDir(QString::fromStdString(m_tmp_dir));
    dir.rename(dir.path(), tmp_dir.path() + "/" + QString::fromStdString(name));

    m_tmp_dir = tmp_dir.path().toStdString() + "/" + name + "/";
    m_dir_bookmarks = m_tmp_dir + "Bookmarks/";
    m_tmp_file = m_tmp_dir + name + ".vian";
}
