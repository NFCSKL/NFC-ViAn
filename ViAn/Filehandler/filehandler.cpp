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
        this->work_space = "C:/ViAn/";
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
void FileHandler::set_workspace(std::string new_work_space){
    this->work_space = new_work_space;
    //save_workspace();
}

/**
 * @brief FileHandler::create_project
 * creates project and associated files.
 * @param std::string name
 * @return Project* created project
 */
Project* FileHandler::create_project(std::string proj_name){
    Project* proj =  new Project(this->project_id, proj_name);
    this->projects.insert(std::make_pair(proj->id, proj));
    save_project(proj, this->work_space,Json);
    this->project_id++;
    return proj;
}

/**
 * @brief FileHandler::create_directory
 * @param dir_path
 * @return unique directory ID
 */
ID FileHandler::create_directory(std::string dir_path){
    this->last_error = make_dir(dir_path); //varying implementation, OS dependant
    ID id = this->add_dir(dir_path);
    return id;
}

/**
 * @brief FileHandler::delete_directory
 * @param id
 * @return errorcode, if deletion was done code is 0;
 * otherwise see OS relevant directoryfile.
 */
FH_ERROR FileHandler::delete_directory(ID id){
    FH_ERROR err = remove_dir(this->get_dir(id)); //varying implementation, OS dependant
    return err;
}

/**
 * @brief FileHandler::save_project
 * @param id
 */
void FileHandler::save_project(ID id){
    this->save_project(get_project(id),this->work_space, FileHandler::SaveFormat::Json); // get project and save it
}

/**
 * @brief FileHandler::save_project
 * @param proj
 * @param dir_path
 * @param save_format
 * @return
 */
bool FileHandler::save_project(Project* proj, std::string dir_path, FileHandler::SaveFormat save_format){
    std::string file_path = dir_path + proj->name;
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

Project* FileHandler::load_project(std::string full_project_path){
    return load_project(full_project_path, Json);
}

Project* FileHandler::load_project(std::string full_project_path, FileHandler::SaveFormat save_format){
    QFile load_file(save_format == Json
        ? QString::fromStdString(full_project_path)
        : QString::fromStdString(full_project_path));

    if (!load_file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QByteArray save_data = load_file.readAll();

    QJsonDocument load_doc(save_format == Json
        ? QJsonDocument::fromJson(save_data)
        : QJsonDocument::fromBinaryData(save_data));

    Project* proj = new Project();
    proj->saved = true;

    proj->read(load_doc.object());
    proj->id = this->project_id++;
    return proj;
}

Project* FileHandler::load_project(std::string proj_name, std::string dir_path){
    return new Project(this->project_id++, "dummy");
}

FH_ERROR FileHandler::delete_project(Project* proj){
    return 0;
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

ID FileHandler::create_file(std::string file_name, ID dir_id){
    std::ofstream f;
    std::string file_path = this->get_dir(dir_id)+"/"+file_name;
    f.open(file_path.c_str());
    return this->add_file(file_path);
  }

/**
 * @todo make threadsafe
 * @brief FileHandler::delete_file
 * delete application tracked file
 * @param ID file id
 */
 FH_ERROR FileHandler::delete_file(ID id){
    std::string file = this->get_file(id);
    return std::remove(file.c_str());
 }

 /**
  * @todo make threadsafe
  * @brief FileHandler::write_file
  *  Write given text to an application tracked file
  * @param ID file id, std::string text
  * @return void
  */
 void FileHandler::write_file(ID id, std::string text, WRITE_OPTION opt){
    std::string file_name = this->get_file(id);
    std::ofstream f;
    switch(opt){
    case WRITE_OPTION::OVERWRITE:
        f.open(file_name.c_str());
        break;
    case WRITE_OPTION::APPEND:
        f.open(file_name.c_str(), std::ios::in | std::ios::out | std::ios::ate);
        break;
    default:
        return; // no open file
        break;
    }
    if(f.is_open()) f << text.c_str();
 }

 /**
  * @brief FileHandler::read_file
  *  Read given lenght of lines to buffer from application
  *  tracked file. OBS! If number of lines exceeds =>
  *  reads to end of file (EOF)
  *  @param ID file id, std::string text
  *  @return voi
  */
 void FileHandler::read_file(ID id,  std::string& buf, int lines_to_read){
     std::ifstream f(this->get_file(id), std::ios::in);
     std::string temp;
     if(f.is_open()){
         while(lines_to_read-- && std::getline(f, temp)){
            buf += temp;
         }
     }
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
 std::string FileHandler::get_file(ID id){
    this->file_map_lock.lock();
    std::string file = this->file_map.at(id);
    this->file_map_lock.unlock();
    return file;
 }

 /**
  * @brief FileHandler::get_dir
  * @param ID directory id
  * @return directory path
  */
 std::string FileHandler::get_dir(ID id){
    this->dir_map_lock.lock();
    std::string dir = this->dir_map.at(id);
    this->dir_map_lock.unlock();
    return dir;
 }

 /**
  * @brief FileHandler::add_projectr
  * @param std::pari<<ID, Project*> pair
  * @return void
  */
 void FileHandler::add_project(std::pair<ID,Project*> pair){
    this->proj_map_lock.lock();
    this->projects.insert(pair);
    this->proj_map_lock.unlock();

 }

 /**
  * @brief FileHandler::add_file
  * @param std::string file_path
  * @return unique file identifier
  */
ID FileHandler::add_file(std::string file_path){
    add_file(this->file_id, file_path);
    return this->file_id++;
 }

/**
 * @brief FileHandler::add_file
 * @param id
 * @param file_path
 */
void FileHandler::add_file(ID id ,std::string file_path){
    std::pair<ID,std::string> pair = std::make_pair(id, file_path);
    this->file_map_lock.lock();
    this->file_map.insert(pair);
    this->file_map_lock.unlock();
}

 /**
  * @brief FileHandler::add_dir
  * @param std::string dir_path
  * @return unique directory identifier
  */
ID FileHandler::add_dir(std::string dir_path){
    std::pair<ID,std::string> pair = std::make_pair(this->dir_id, dir_path);
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
    return projfiles_equal(*proj.files , *proj2.files) && //probably unnecessary as projfiles have projname followed by default suffix
           proj.name == proj2.name &&
           video_equals;
}

/**
 * @brief FileHandler::projfiles_equal
 * @param pf
 * @param pf2
 * @return true if files are the same paths
 */
bool FileHandler::projfiles_equal(ProjFiles& pf, ProjFiles& pf2){
    return dirs_equal(pf.dir, pf2.dir) &&
        this->files_equal(pf.f_proj, pf2.f_proj) &&
        this->files_equal(pf.f_analysis, pf2.f_analysis)&&
        this->files_equal(pf.f_drawings, pf2.f_drawings) &&
        this->files_equal(pf.f_videos, pf2.f_videos);
}

/**
 * @brief FileHandler::files_equal
 * @param id
 * @param id2
 * @return
 */
bool FileHandler::files_equal(ID id, ID id2){
    return this->get_file(id) == this->get_file(id2);
}

/**
 * @brief FileHandler::dirs_equal
 * @param id
 * @param id2
 * @return true if dirs are same path
 */
bool FileHandler::dirs_equal(ID id, ID id2){
    return this->get_dir(id) == this->get_dir(id2);
}
