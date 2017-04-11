#include "filehandler.h"

/**
 * @brief FileHandler::FileHandler
 */
FileHandler::FileHandler() {
    this->project_id = 0; // zero out counter ids
    this->file_id = 0;
    this->dir_id = 0;
    this->last_error = 0;
    #ifdef _WIN32
        this->work_space = create_directory("C:/ViAn/");
    #elif __APPLE__
        this->work_space = "/Applications/";
    #elif __unix__
        this->work_space = "~/";
    #endif

    //ID id = add_file("ViAn_config.txt"); Will be used to store current workspace and other run-to-run coonstans
}
/**
 * @todo save workspace to file
 * @brief FileHandler::set_workspace
 * @param newWorkSpace
 */
void FileHandler::set_work_space(std::string new_work_space){
    //this->work_space = new_work_space;
    //save_workspace();
}

QDir FileHandler::get_work_space()
{
    return this->get_dir(this->work_space);
}

/**
 * @brief FileHandler::create_project
 * creates project and associated files.
 * @param std::string name
 * @return Project* created project
 */
Project* FileHandler::create_project(QString proj_name, std::string dir_path){
    Project* proj =  new Project(this->project_id, proj_name.toStdString());   
    ID dir;
    if(dir_path != "")
        dir = create_directory(QString::fromStdString(dir_path));
    else if(dir_path != "" && !proj->saved)
        dir = this->work_space;
    else if(dir_path == "" && proj->saved)
        dir = proj->dir;
    else
        dir = this->work_space;
    proj->dir = dir;
    add_project(proj);
    save_project(proj, dir ,Json);
    return proj;
}

/**
 * @brief FileHandler::create_directory
 * @param dir_path
 * @return unique directory ID
 */
ID FileHandler::create_directory(QString dir_path){
    QDir dir (QDir::root());
    last_error = dir.mkpath(dir_path);
    if(!last_error){
        qWarning("Could not create directory %s",dir_path.toStdString().c_str());
    }
    dir.setPath(dir_path);
    ID id = this->add_dir(dir);
    return id;
}

/**
 * @brief FileHandler::delete_directory
 * @param id
 * @return errorcode, if deletion was done code is 0;
 * otherwise see OS relevant directoryfile.
 */
FH_ERROR FileHandler::delete_directory(ID id){
    QDir temp = this->get_dir(id);
    if(temp.rmdir(temp.absolutePath())){
        this->dir_map.erase(id);
        return 0;
    }
    return 1;

}

/**
 * @brief FileHandler::save_project
 * @param id
 */
void FileHandler::save_project(ID id){
    Project* proj = get_project(id);
    this->save_project(proj, this->work_space, FileHandler::SaveFormat::Json); // get project and save it
}
void FileHandler::save_project(Project* proj){
    this->save_project(proj, this->work_space, FileHandler::SaveFormat::Json);
}

/**
 * @brief FileHandler::save_project
 * @param proj
 * @param dir_path
 * @param save_format
 * @return
 */
bool FileHandler::save_project(Project* proj, ID dir_id, FileHandler::SaveFormat save_format){
    QDir dir = get_dir(dir_id);
    std::string file_path = dir.filePath(QString::fromStdString(proj->name)).toStdString();
    QFile save_file(save_format == Json
                    ? QString::fromStdString(file_path + ".json")
                    : QString::fromStdString(file_path + ".dat"));
    if(!save_file.open(QIODevice::WriteOnly)){
        return false;
    }
    QJsonObject json_proj;
    proj->write(json_proj);
    QJsonDocument save_doc(json_proj);
    save_file.write(save_format == Json
            ? save_doc.toJson()
            : save_doc.toBinaryData());
    return true;
}
/**
 * @brief FileHandler::load_project
 * @param full_project_path
 * @return loaded Project
 * Public load function
 */
Project* FileHandler::load_project(std::string full_project_path){
    return load_project(full_project_path, Json); // Decide format internally, here for flexibility
}
/**
 * @brief FileHandler::load_project
 * @param full_project_path
 * @param save_format
 * @return loaded Project
 * Loads project from json file and returns it
 */
Project* FileHandler::load_project(std::string full_path, FileHandler::SaveFormat save_form){
    QFile load_file(save_form == Json
        ? QString::fromStdString(full_path)
        : QString::fromStdString(full_path));

    if (!load_file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return nullptr;
    }
    QByteArray save_data = load_file.readAll();

    QJsonDocument load_doc(save_form == Json
        ? QJsonDocument::fromJson(save_data)
        : QJsonDocument::fromBinaryData(save_data));

    Project* proj = new Project();
    proj->saved = true;

    proj->read(load_doc.object());
    proj->id = add_project(proj);
    proj->dir = add_dir(QDir(QString::fromStdString(full_path.substr(0, full_path.find_last_of("/")))));
    return proj;
}

FH_ERROR FileHandler::delete_project(ID proj_id){
    Project* temp = get_project(proj_id);
    QFile file(get_dir(temp->dir).absoluteFilePath(QString::fromStdString(temp->name + ".json")));
    if(this->projects.erase(proj_id)){
        file.remove();
        delete temp;
        return 0;
    }
    return 1;
}

/**
 * @todo make threadsafe
 * @brief FileHandler::add_video
 * @param Project*,string file_path
 *
 * string
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
    proj->remove_video(vid_id); // Remove ´the video from project
}

 /**
  * @brief FileHandler::create_file
  * create a file by given name in already excisting
  * application tracked directory
  * @param std::string file name, ID directory id
  */

ID FileHandler::create_file(QString file_name, QDir dir){
    return this->add_file(dir.absoluteFilePath(file_name)); // File created
  }

/**
 * @brief FileHandler::delete_file
 * delete application tracked file
 * @param ID file id
 */
 FH_ERROR FileHandler::delete_file(ID id){
    QFile file(this->get_file(id));
    file.remove();
    if(this->file_map.erase(id)) return 0;
    return 1; // Error
 }

 /**
  * @todo make threadsafe
  * @brief FileHandler::write_file
  *  Write given text to an application tracked file
  * @param ID file id, std::string text
  * @return void
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
  *  Read given lenght of lines to buffer from application
  *  tracked file. OBS! If number of lines exceeds =>
  *  reads to end of file (EOF)
  *  @param ID file id, std::string text
  *  @return voi
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
  * Getter
  * @param ID project id
  * @return Project*
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
  */
 ID FileHandler::add_project(Project* proj){
     add_project(this->project_id, proj);
     return this->project_id++;
 }

 /**
  * @brief FileHandler::add_project
  * @param std::pari<<ID, Project*> pair
  * @return void
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
  */
ID FileHandler::add_file(QString file){
    add_file(this->file_id, file);
    return this->file_id++;
 }

/**
 * @brief FileHandler::add_file
 * @param id
 * @param file_path
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
  */
ID FileHandler::add_dir(QDir dir){
    std::pair<ID,QDir> pair = std::make_pair(this->dir_id, dir);
    this->dir_map_lock.lock();
    this->dir_map.insert(pair);
    this->dir_map_lock.unlock();
    return this->dir_id++;
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
               [](const std::pair<ID,Video*> v, const std::pair<ID,Video*> v2){return *(v.second) == *(v2.second);}); // lambda function comparing using video==
                                                                        // by dereferencing pointers in vector
    return proj.name == proj2.name &&
           video_equals;
}

