#include "project.h"

#include "bookmark.h"
#include "GUI/Bookmark/bookmarkcategory.h"
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
 * @brief Project::add_video_project
 * @return Video ID to be used for identifying the video.
 */
ID Project::add_video_project(VideoProject *vid_proj){
    vid_proj->set_project(this);
    m_videos.push_back(vid_proj);
    int id;
    if (vid_proj->get_id() == -1) {
        vid_proj->set_id(m_vid_count);
        id = m_vid_count;
        m_vid_count++;
    } else {
        id = vid_proj->get_id();
    }
    m_unsaved_changes = true;
    return id;
}

/**
 * @brief Project::remove_video_project
 * @param vid_proj
 * Remove video from videos and delete its contents.
 */
void Project::remove_video_project(VideoProject* vid_proj){
    auto it = std::find(m_videos.begin(), m_videos.end(), vid_proj);
    if (it == m_videos.end()) return;
    delete *it;
    m_videos.erase(it);
    m_unsaved_changes = true;
}

void Project::add_bookmark(Bookmark* bmark) {
    bmark->set_project(this);
    m_bookmarks.push_back(bmark);
    m_unsaved_changes = true;
}

void Project::remove_bookmark(Bookmark* bmark) {
    auto it = std::find(m_bookmarks.begin(), m_bookmarks.end(), bmark);
    if (it == m_bookmarks.end()) return;
    delete *it;
    m_bookmarks.erase(it);
    m_unsaved_changes = true;
    bmark->remove_exported_image();
}

ID Project::add_category(BookmarkCategory* cat) {
    cat->set_project(this);
    m_categories.push_back(cat);
    int id;
    if (cat->get_id() == -1) {
        cat->set_id(m_cat_count);
        id = m_cat_count;
        m_cat_count++;
    } else {
        id = cat->get_id();
    }
    m_unsaved_changes = true;
    return id;
}

void Project::remove_category(BookmarkCategory* cat) {
    auto it = std::find(m_categories.begin(), m_categories.end(), cat);
    if (it == m_categories.end()) return;
    delete *it;
    m_categories.erase(it);
    m_unsaved_changes = true;
}

void Project::add_analysis(AnalysisProxy* ana) {
    m_analyses.push_back(ana);
    m_unsaved_changes = true;
}

void Project::remove_analysis(AnalysisProxy* ana) {
    auto it = std::find(m_analyses.begin(), m_analyses.end(), ana);
    if (it == m_analyses.end()) return;
    m_analyses.erase(it);
    m_unsaved_changes = true;
}

/**
 * @brief Project::set_unsaved
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
        for (auto bmark : m_bookmarks) {
            bmark->reset_root_dir(m_dir);
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
    bool video_projects_saved = std::all_of(m_videos.begin(), m_videos.end(),
                                            [](VideoProject* vp){return vp->is_saved();});
    bool bookmarks_saved = std::all_of(m_bookmarks.begin(), m_bookmarks.end(),
                                            [](Bookmark* bm){return bm->is_saved();});
    bool categories_saved = std::all_of(m_categories.begin(), m_categories.end(),
                                            [](BookmarkCategory* cat){return cat->is_saved();});
    return !m_unsaved_changes && video_projects_saved && bookmarks_saved && categories_saved;
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
    m_vid_count = json["vid IDs"].toInt();
    m_cat_count = json["cat IDs"].toInt();

    // Read videos from json
    QJsonArray json_vid_projs = json["videos"].toArray();
    for (int i = 0; i < json_vid_projs.size(); ++i) {
        QJsonObject json_vid_proj = json_vid_projs[i].toObject();
        VideoProject* v = new VideoProject();
        add_video_project(v);
        v->read(json_vid_proj);
        v->reset_root_dir(m_dir);
    }

    // Read bookmarks from json
    QJsonArray json_bookmarks = json["bookmarks"].toArray();
    for (int i = 0; i < json_bookmarks.size(); ++i) {
        QJsonObject json_bmark = json_bookmarks[i].toObject();
        Bookmark* bmark = new Bookmark();
        bmark->read(json_bmark);
        add_bookmark(bmark);
        bmark->reset_root_dir(m_dir);
    }

    // Read categories from json
    QJsonArray json_cats = json["categories"].toArray();
    for (int i = 0; i < json_cats.size(); ++i) {
        QJsonObject json_category = json_cats[i].toObject();

        QString name = json_category["name"].toString();
        int index = json_category["index"].toInt();
        int id = json_category["id"].toInt();

        BookmarkCategory* new_category = new BookmarkCategory(name, 1);
        new_category->set_index(index);
        new_category->set_id(id);
        add_category(new_category);
        new_category->m_unsaved_changes = false;
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
    json["root_dir"] = m_dir;
    json["vid IDs"] = m_vid_count;
    json["cat IDs"] = m_cat_count;

    // Write Videos to json
    QJsonArray json_proj;
    for(auto it = m_videos.begin(); it != m_videos.end(); it++){
        QJsonObject json_vid_proj;
        (*it)->write(json_vid_proj);
        json_proj.append(json_vid_proj);
    }
    json["videos"] = json_proj;

    // Write Bookmarks to json
    QJsonArray json_bmarks;
    for(auto it = m_bookmarks.begin(); it != m_bookmarks.end(); it++){
        QJsonObject json_bmark;
        (*it)->write(json_bmark);
        json_bmarks.append(json_bmark);
    }
    json["bookmarks"] = json_bmarks;

    // Write Categories to json
    QJsonArray json_cats;
    for(auto it = m_categories.begin(); it != m_categories.end(); it++){
        QJsonObject json_category;
        json_category["name"] = (*it)->get_name();
        json_category["index"] = (*it)->get_index();
        json_category["id"] = (*it)->get_id();
        json_cats.append(json_category);
        (*it)->m_unsaved_changes = false;
    }
    json["categories"] = json_cats;

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
std::vector<VideoProject*> &Project::get_videos() {
    return m_videos;
}

std::vector<Bookmark*> &Project::get_bookmarks() {
    return m_bookmarks;
}

std::vector<BookmarkCategory*> &Project::get_categories() {
    return m_categories;
}

std::vector<AnalysisProxy*> &Project::get_analyses() {
    return m_analyses;
}

/**
 * @brief Project::get_video_project
 * Get the videoproject with the ID from the list of videoproject.
 * If the videoproject is not found return nullptr.
 * @param id
 * @return
 */
VideoProject* Project::get_video_project(int id) {
    for (auto vp : m_videos) {
        if (vp->get_id() == id) {
            return vp;
        }
    }
    return nullptr;
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
