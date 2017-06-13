#include "projectmanager.h"

ProjectManager::ProjectManager()
{

}
/**
 * @brief FileHandler::add_project
 * @param proj
 * Adds project to map.
 */
ID ProjectManager::add_project(Project* proj){
    add_project(this->project_id, proj);
    return this->project_id++;
}

/**
 * @brief FileHandler::add_project
 * @param std::pair<<ID, Project*> pair
 * @return void
 * Adds project to projects, locks in doing so.
 */
void ProjectManager::add_project(ID id, Project* proj){
   std::pair<ID,Project*> pair = std::make_pair(id,proj);
   this->proj_map_lock.lock();
   this->projects.insert(pair);
   this->proj_map_lock.unlock();

}

/**
 * @brief FileHandler::get_project
 * @param ID project id
 * @return Project*
 * Gets project by ID, locks in doing so.
 */
Project* ProjectManager::get_project(ID id){
   this->proj_map_lock.lock();
   Project* p = this->projects.at(id);
   this->proj_map_lock.unlock();
   return p;
}

/**
 * @brief FileHandler::delete_project
 * @param proj_id
 * @return Deletes project entirely
 * Deletes project and frees allocated memory.
 */
bool ProjectManager::delete_project(ID proj_id){
    Project* temp = get_project(proj_id);
    this->proj_map_lock.lock();
    if(this->projects.erase(proj_id)){
        close_project(temp->id);
// Logic which remove system files should not be here, should be deleted separately
//        temp->delete_artifacts();
//        QFile file;
//        if(this->save_format == BINARY )  file.setFileName(get_dir(temp->dir).absoluteFilePath(QString::fromStdString(temp->name + ".dat")));
//        else file.setFileName((get_dir(temp->dir).absoluteFilePath(QString::fromStdString(temp->name + ".json"))));
//        file.remove();
//        delete_directory(temp->dir_bookmarks);
//        delete_directory(temp->dir);
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
ID ProjectManager::add_video(Project* proj, std::string file_path){
    Video* v = new Video(file_path);
    return proj->add_video(v); // video id set in proj->add_video
}

/**
 * @brief FileHandler::remove_video_from_project
 * @param proj_id
 * @param vid_id
 * Removes video from project according to given ids.
 */
void ProjectManager::remove_video_from_project(ID proj_id, ID vid_id){
    Project* proj = this->get_project(proj_id); // get Project object from id
    proj->remove_video_project(vid_id); // Remove ´the video from project
}

/**
 * @brief FileHandler::load_project
 * @param full_project_path
 * @return loaded Project
 * Public load function
 * Used for simplicity of call and
 * for hiding save format.
 */
Project* ProjectManager::load_project(std::string full_project_path){
// maybe handle this in project class? overriding savveable?
//     Project* proj = new Project(this);
//     load_saveable(proj, full_project_path, this->save_format); // Decide format internally, here for flexibility
//     proj->save_project();
//     proj->id = add_project(proj);
//     return proj;
}

/**
 * @brief FileHandler::save_project
 * @param id
 * Save projects, exposed interface.
 * Here for simplicity of call as well as hiding save format.
 */
void ProjectManager::save_project(ID id){
    Project* proj = get_project(id);
    save_project(proj); // get project and save it
}

/**
 * @brief FileHandler::save_project
 * @param proj
 * Exposed interface, added for simplicity of call when
 * project pointer is still available
 */
void ProjectManager::save_project(Project *proj){
    proj->save_saveable(proj, proj->dir, proj->save_format);
    proj->save_project();
}

/**
 * @brief FileHandler::open_project
 * @param id
 * Open project by id.
 */
void ProjectManager::open_project(ID id){
    this->open_projects.push_back(id);
}

/**
 * @brief FileHandler::close_project
 * @param id
 * Close project by id.
 */
void ProjectManager::close_project(ID id){
    open_projects.erase(std::remove(open_projects.begin(), open_projects.end(), id), open_projects.end());
}

/**
 * @brief FileHandler::proj_equals
 * @param proj
 * @param proj2
 * @return true if project contents are the same
 */
bool ProjectManager::proj_equals(Project& proj, Project& proj2){
    bool video_equals =  std::equal(proj.get_videos().begin(), proj.get_videos().end(),
               proj2.get_videos().begin(),
               [](const std::pair<ID,VideoProject*> v, const std::pair<ID,VideoProject*> v2){return *(v.second->get_video()) == *(v2.second->get_video());}); // lambda function comparing using video==
                                                                                                                      // by dereferencing pointers in vector
    return proj.name == proj2.name &&
           video_equals;
}
