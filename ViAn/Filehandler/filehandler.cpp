#include "filehandler.h"

/**
 * @brief FileHandler::FileHandler
 * Initialize filehandler and set
 * default value for workspace
 */
FileHandler::FileHandler() {
    this->project_id = 0; // zero out counter ids
    this->file_id = 0;
    this->dir_id = 0;
    this->last_error = false;    
    open_projects.clear();
    #ifdef _WIN32
        this->work_space = create_directory("C:/");
    #elif __APPLE__
        this->work_space =  create_directory("/Applications/");
    #elif __unix__
        this->work_space =  create_directory("~/");
    #endif
    load();
}

/**
 * @brief FileHandler::~FileHandler
 * Save filehandler state when destroyed.
 * Deletes projects.
 */
FileHandler::~FileHandler(){
    // Delete projects
    for(auto it = projects.begin(); it != projects.end(); it++){
        delete it->second;
    }
    save();
}

/**
 * @brief FileHandler::save
 * Save filehandler.
 */
void FileHandler::save(){
    QDir dir;
    dir.cd(".");
    this->save_name = dir.absoluteFilePath("state").toStdString();
    save_saveable(this,add_dir(dir), JSON);
}

/**
 * @brief FileHandler::load
 * Load filehandler.
 */
void FileHandler::load(){
    QDir dir;
    dir.cd(".");
    this->save_name = dir.absoluteFilePath("state.json").toStdString();
    load_saveable(this, this->save_name, JSON);
}

/**
 * @brief FileHandler::open_project
 * @param id
 * Open project by id.
 */
void FileHandler::open_project(ID id){
    this->open_projects.push_back(id);
}

/**
 * @brief FileHandler::close_project
 * @param id
 * Close project by id.
 */
void FileHandler::close_project(ID id){
    open_projects.erase(std::remove(open_projects.begin(), open_projects.end(), id), open_projects.end());
}

/**
 * @todo save workspace to file
 * @brief FileHandler::set_workspace
 * @param new_work_space
 * Change default work_space, ie where project files are saved.
 */
void FileHandler::set_work_space(std::string new_work_space){
    this->work_space = create_directory(QString::fromStdString(new_work_space));
}

/**
 * @brief FileHandler::get_work_space
 * @return default work_space
 */
QDir FileHandler::get_work_space(){
    return this->get_dir(this->work_space);
}

/**
 * @brief FileHandler::read
 * @param json
 * Reads filehandler from a json object.
 */
void FileHandler::read(const QJsonObject &json){
    QJsonArray json_projs = json["open"].toArray();
    for(int i = 0; i != json_projs.size(); i++){
        QJsonObject json_path = json_projs[i].toObject();
        std::string path = json_path["path"].toString().toStdString();
        open_project(load_project(path)->id);
    }
}
/**
 * @brief FileHandler::write
 * @param json
 * Writes filehandler to a json object
 */
void FileHandler::write(QJsonObject &json){
    QJsonArray json_projs;
    for (auto it = open_projects.begin();  it != open_projects.end(); it++) {
        QJsonObject json_path;
        ID id = *it;
        Project* proj = get_project(id);
        QDir dir = get_dir(proj->dir);
        QString path = dir.absoluteFilePath((proj->name + ".json").c_str());
        json_path["path"] = path;
        json_projs.append(json_path);
    }
    json["open"] = json_projs;
}

/**
 * @brief FileHandler::create_project
 * creates project and associated files.
 * @param std::string name
 * @return Project* created project
 */
Project* FileHandler::create_project(QString proj_name, std::string dir_path, std::string vid_path){
    Project* proj =  new Project(this->project_id, proj_name.toStdString());   
    ID root_dir;
    if(dir_path != "")                          //Directory name provided
        root_dir = create_directory(QString::fromStdString(dir_path));
    else if(dir_path == "" && proj->dir == -1)  // No directory name provided, project has no directory.
        root_dir = this->work_space;                 // Default save location to workspace
    else if(dir_path == "" && proj->dir != -1)  // No Directory provided and project previosuly saved
        root_dir = proj->dir;                        // Use present save location
    else
        root_dir = this->work_space;    
    proj->bookmark_dir = create_directory(get_dir(root_dir).absoluteFilePath(QString::fromStdString(proj->name+"/Bookmarks")));
    proj->dir = create_directory(get_dir(root_dir).absoluteFilePath(QString::fromStdString(proj->name)));
    if(vid_path != "")
        proj->dir_videos = create_directory(get_dir(root_dir).absoluteFilePath(QString::fromStdString(vid_path)));

    add_project(proj);                          // Add project to file sytstem
    save_project(proj);                         // Save project file
    open_project(proj->id);                     // Open project
    return proj;
}

/**
 * @brief FileHandler::create_directory
 * @param dir_path
 * @return id
 * Creates a directory and all directories needed for that directory, ie
 * "C:/THIS/IS/A/PATH/" will create THIS, IS, A and PATH directories if these do not exist.
 */
ID FileHandler::create_directory(QString dir_path){
    QDir dir (QDir::root());
    last_error = !dir.mkpath(dir_path);
    if(last_error){
        qWarning("Could not create directory %s",dir_path.toStdString().c_str());
    }

    dir.setPath(dir_path);
    ID id = this->add_dir(dir);
    return id;
}

/**
 * @brief FileHandler::delete_directory
 * @param id
 * @return bool, success.
 * Deletes a given directory
 */
bool FileHandler::delete_directory(ID id){
    QDir temp = this->get_dir(id);
    this->dir_map_lock.lock(); // Order important, locked in getter
    if(temp.rmdir(temp.absolutePath())){
        this->dir_map.erase(id);
        this->dir_map_lock.unlock();
        return true;
    }
    this->dir_map_lock.unlock();
    return false;

}

/**
 * @brief FileHandler::save_project
 * @param id
 * Save projects, exposed interface.
 * Here for simplicity of call as well as hiding save format.
 */
void FileHandler::save_project(ID id){
    Project* proj = get_project(id);
    this->save_saveable(proj, proj->dir, FileHandler::SAVE_FORMAT::JSON); // get project and save it
}

/**
 * @brief FileHandler::save_project
 * @param proj
 * Exposed interface, added for simplicity of call when
 * project pointer is still available
 */
void FileHandler::save_project(Project *proj){
    this->save_saveable(proj, proj->dir, FileHandler::SAVE_FORMAT::JSON);
}

/**
 * @brief FileHandler::save_saveable
 * @param savable
 * @param dir_path
 * @param save_format
 * @return Saves a Json file to provided directory
 */
bool FileHandler::save_saveable(Saveable *saveable, ID dir_id, FileHandler::SAVE_FORMAT save_format){
    QDir dir = get_dir(dir_id);
    std::string file_path = dir.absoluteFilePath(QString::fromStdString(saveable->save_name)).toStdString();
    QFile save_file(save_format == JSON
                    ? QString::fromStdString(file_path + ".json")
                    : QString::fromStdString(file_path + ".dat"));    
    if(!save_file.open(QIODevice::WriteOnly)){
        qWarning("Couldn't open save file.");
        return false;
    }
    QJsonObject json_saveable;
    saveable->write(json_saveable);
    QJsonDocument save_doc(json_saveable);
    save_file.write(save_format == JSON
            ? save_doc.toJson()
            : save_doc.toBinaryData());
    return true;
}

/**
 * @brief FileHandler::load_project
 * @param full_project_path
 * @return loaded Project
 * Public load function
 * Used for simplicity of call and
 * for hiding save format.
 */
Project* FileHandler::load_project(std::string full_project_path){
     Project* proj = new Project();
     load_saveable(proj, full_project_path, JSON); // Decide format internally, here for flexibility
     proj->saved = true;
     proj->id = add_project(proj);
     proj->dir = add_dir(QDir(QString::fromStdString(full_project_path.substr(0, full_project_path.find_last_of("/")))));
     proj->bookmark_dir = add_dir(QDir(QString::fromStdString(full_project_path.substr(0, full_project_path.find_last_of("/")) + "/Bookmarks")));
     proj->dir_videos = this->work_space;
     return proj;
}

/**
 * @brief FileHandler::load_savable
 * @param full savable path
 * @param save_format
 * @return loaded Project
 * Loads project from json file and returns it
 */
Saveable *FileHandler::load_saveable(Saveable *saveable, std::string full_path, FileHandler::SAVE_FORMAT save_form){
    QFile load_file(save_form == JSON
        ? QString::fromStdString(full_path)
        : QString::fromStdString(full_path));
    if (!load_file.open(QIODevice::ReadOnly)) {        
        qWarning("Couldn't open load file %s. ", load_file.fileName().toStdString().c_str());
        return nullptr;
    }
    QByteArray save_data = load_file.readAll();

    QJsonDocument load_doc(save_form == JSON
        ? QJsonDocument::fromJson(save_data)
        : QJsonDocument::fromBinaryData(save_data));
    saveable->read(load_doc.object());
    return saveable;
}

/**
 * @brief FileHandler::delete_project
 * @param proj_id
 * @return Deletes project entirely
 * Deletes project and frees allocated memory.
 */
bool FileHandler::delete_project(ID proj_id){
    Project* temp = get_project(proj_id);
    this->proj_map_lock.lock();    
    if(this->projects.erase(proj_id)){
        close_project(temp->id);
        temp->delete_artifacts();
        QFile file (get_dir(temp->dir).absoluteFilePath(QString::fromStdString(temp->name + ".json")));
        file.remove();
        delete_directory(temp->bookmark_dir);
        delete_directory(temp->dir);
        delete temp;        
        this->proj_map_lock.unlock();
        return true;
    }
    this->proj_map_lock.unlock();    
    return false;
}

/**
 * @todo make threadsafe
 * @brief FileHandler::add_video
 * @param Project*,string file_path
 * Add a video file_path to a given project.
 * Creates Video object which is accessed further by returned id.
 */
ID FileHandler::add_video(Project* proj, std::string file_path){
    Video* v = new Video(file_path);
    return proj->add_video(v); // video id set in proj->add_video
}

/**
 * @brief FileHandler::remove_video_from_project
 * @param proj_id
 * @param vid_id
 * Removes video from project according to given ids.
 */
void FileHandler::remove_video_from_project(ID proj_id, ID vid_id){
    Project* proj = this->get_project(proj_id); // get Project object from id
    proj->remove_video_project(vid_id); // Remove ´the video from project
}

/**
 * @brief FileHandler::create_file
 * @param std::string file name, ID directory id
 * create a file by given name in already existing
 * application tracked directory.
 * Note that the file is not actually created
 * in the file system until the file is written to.
 */
ID FileHandler::create_file(QString file_name, QDir dir){
    return this->add_file(dir.absoluteFilePath(file_name)); // File created
  }

/**
 * @brief FileHandler::delete_file
 * delete application tracked file
 * @param ID file id
 */
 bool FileHandler::delete_file(ID id){
    QFile file(this->get_file(id));
    file.remove();
    return this->file_map.erase(id);
 }

 /**
  * @todo make threadsafe
  * @brief FileHandler::write_file
  *  Write given text to an application tracked file
  * @param ID file id, std::string text
  * @return void
  * Write given text with OPTION opt, supported OPTIONs are append and overwrite.
  */
 void FileHandler::write_file(ID id, QString text, WRITE_OPTION opt){
    QFile file;
    file.setFileName(this->get_file(id));

    switch(opt){
    case OVERWRITE:
        if(!file.open(QIODevice::ReadWrite | QIODevice::QIODevice::Truncate |QIODevice::Text))return; // Empty file
        break;
    case APPEND:
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append)) return; // File can not be written
        break;
    default:
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) return; // File can not be written
        break;
    }
    QTextStream out(&file);
    out << text;
    file.close();
 }

 /**
  * @brief FileHandler::read_file
  * @param ID file id, std::string text
  * @return void
  * Read given length of lines to buffer from application
  * tracked file. OBS! If number of lines exceeds lines in file,
  * reads to end of file (EOF)
  */
 void FileHandler::read_file(ID id,  QString& buf, int lines_to_read){
    QFile file (this->get_file(id));
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
        qWarning("File %s could not be read", file.fileName().toStdString().c_str());
        return; // File can not be read
    }
    QTextStream in(&file);
    QString line;
    while(lines_to_read-- && !in.atEnd()){
        line = in.readLine();
        buf.append(line);
    };
    file.close();
 }

 /**
  * @brief FileHandler::get_project
  * @param ID project id
  * @return Project*
  * Gets project by ID, locks in doing so.
  */
 Project* FileHandler::get_project(ID id){
    this->proj_map_lock.lock();
    Project* p = this->projects.at(id);
    this->proj_map_lock.unlock();
    return p;
 }

 /**
  * @brief FileHandler::get_file
  * Getter
  * @param ID project file id
  * @return std::string file_path
  * Get file by ID, locks in doing so.
  */
 QString FileHandler::get_file(ID id){
    this->file_map_lock.lock();
    QString file = this->file_map.at(id);
    this->file_map_lock.unlock();
    return file;
 }

 /**
  * @brief FileHandler::get_dir
  * @param ID directory id
  * @return directory path
  * Gets directory by id, locks in doing so.
  */
 QDir FileHandler::get_dir(ID id){
    this->dir_map_lock.lock();
    QDir dir = this->dir_map.at(id);
    this->dir_map_lock.unlock();
    return dir;
 }

 /**
  * @brief FileHandler::add_project
  * @param proj
  * Adds project to map.
  */
 ID FileHandler::add_project(Project* proj){
     add_project(this->project_id, proj);
     return this->project_id++;
 }

 /**
  * @brief FileHandler::add_project
  * @param std::pair<<ID, Project*> pair
  * @return void
  * Adds project to projects, locks in doing so.
  */
 void FileHandler::add_project(ID id, Project* proj){
    std::pair<ID,Project*> pair = std::make_pair(id,proj);
    this->proj_map_lock.lock();
    this->projects.insert(pair);
    this->proj_map_lock.unlock();

 }

 /**
  * @brief FileHandler::add_file
  * @param std::string file_path
  * @return unique file identifier
  * Adds file to filesystem.
  */
ID FileHandler::add_file(QString file){
    add_file(this->file_id, file);
    return this->file_id++;
 }

/**
 * @brief FileHandler::add_file
 * @param id
 * @param file_path
 * Adds file to file_map, locks in doing so.
 */
void FileHandler::add_file(ID id ,QString file){
    std::pair<ID,QString> pair = std::make_pair(id, file);
    this->file_map_lock.lock();
    this->file_map.insert(pair);
    this->file_map_lock.unlock();
}

 /**
  * @brief FileHandler::add_dir
  * @param std::string dir_path
  * @return unique directory identifier
  * Adds directory to directories, locks in doing so.
  */
ID FileHandler::add_dir(QDir dir){
    add_dir(this->dir_id, dir);
    return this->dir_id++;
 }

/**
 * @brief FileHandler::add_dir
 * @param dir
 * Inserts directory by id in directories.
 */
void FileHandler::add_dir(ID dir_id, QDir dir){
    std::pair<ID,QDir> pair = std::make_pair(dir_id, dir);
    this->dir_map_lock.lock();
    this->dir_map.insert(pair);
    this->dir_map_lock.unlock();
}

/**
 * @brief FileHandler::proj_equals
 * @param proj
    * @param proj2
 * @return true if project contents are the same
 */
bool FileHandler::proj_equals(Project& proj, Project& proj2){
    bool video_equals =  std::equal(proj.videos.begin(), proj.videos.end(),
               proj2.videos.begin(),
               [](const std::pair<ID,VideoProject*> v, const std::pair<ID,VideoProject*> v2){return *(v.second->get_video()) == *(v2.second->get_video());}); // lambda function comparing using video==
                                                                                                                      // by dereferencing pointers in vector
    return proj.name == proj2.name &&
           video_equals;
}
