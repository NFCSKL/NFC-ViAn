#include "filehandler.h"
FileHandler::FileHandler()
{
    this->m_pid = 0;
    this->m_fid = 0;
    this->m_did = 0;

}
/**
 * @brief FileHandler::createProject
 * creates project and associated files.
 * @param std::string name
 * @return Project* created project
 */
Project* FileHandler::createProject(std::string projName){
    Project* proj =  new Project(this->m_pid, projName);
    this->m_projects.insert(std::make_pair(proj->m_id, proj));
    saveProject(proj);
    this->m_pid++;
    return proj;
}
ID FileHandler::createDirectory(std::string dirpath){
    int err = makeDir(dirpath);
    this->addDir(std::make_pair(this->m_did, dirpath));
    return this->m_did++;
}

FH_ERROR FileHandler::deleteDirectory(std::string dirpath){
    FH_ERROR err = removeDir(dirpath);
    return err;
}



/**
 * @todo unfinished, needs full project structure
 * and program to file parser to finish
 * @brief FileHandler::saveProject
 * @param Project* name
 * Creates project and associated files.
 * @return void
 */
void FileHandler::saveProject(Project* proj){
    std::string fileName = proj->m_name + std::string(".txt"); //filename
    ID dirID = createDirectory(std::string(WORKSPACE) + "/"+ proj->m_name);//project directory
    proj->m_dir = dirID;
    proj->m_file = createFile(fileName, dirID); //create project file

    updateProjFile(proj);

}
/**
 * @todo unfinished, will be released with parser
 * however, is needed for creating
 * @brief FileHandler::saveProject
 * @param Project* name
 * Creates project and associated files.
 * @return void
 */
void FileHandler::updateProjFile(Project* proj){
    std::string projstr = "";
    projstr += "Name:"+proj->m_name + "\n";
    std::vector<Video> vids = proj->m_videos;
    for (std::vector<Video>::iterator v = vids.begin();v != vids.end(); ++v) {
        projstr += "V" + std::to_string(v->id) + ":" + this->m_fileMap.at(v->id) + "\n";
    }
    writeFile(proj->m_file, projstr);
}

/**
 * @todo unfinished, will be released with parser, needs full
 * project structure and file to program parser to finish.
 * @brief FileHandler::loadProject
 * @param std::string
 * Load a project object from a given filepath
 */
Project* FileHandler::loadProject(std::string filePath){
    Project* proj;
    std::ifstream f(filePath);
    std::string content = "";
    std::string line = "";
    proj = createProject(line.substr(0, line.find(":")));
    //while(proj << f); //pretty magic, see << definition in proj.cpp
}

/**
 * @brief FileHandler::deleteProject
 * Deletes project, its associated files and contents.
 * OBS! This operation is as of now irreversible
 * @param Project*
 * @return int errorcode
 */
FH_ERROR FileHandler::deleteProject(Project* proj){
    deleteFile(proj->m_file);
    return deleteDirectory(this->getDir(proj->m_dir));

}
/**
 * @todo make threadsafe
 * @brief FileHandler::addVideo
 * @param Project*, std::string
 * Add a video filepath to a given project.
 * Creates Video object which is accessed further by returned id.
 */
void FileHandler::addVideo(Project* proj, std::string filePath){
    Video v (this->m_fid, filePath);
    proj->addVideo(v);
    this->addFile(std::make_pair(this->m_fid++,filePath));
}

 /**
  * @brief FileHandler::createFile
  * create a file by given name in already excisting
  * application tracked directory
  * @param std::string file name, ID directory id
  */

ID FileHandler::createFile(std::string filename, ID dirID){
     std::ofstream f;
     std::string filePath = this->getDir(dirID)+"/"+filename;
     f.open(filePath.c_str());
     this->addFile(std::make_pair(this->m_fid, filePath));
     return this->m_fid++;
  }
/**
 * @todo make threadsafe
 * @brief FileHandler::deleteFile
 * delete application tracked file
 * @param ID file id
 */
 FH_ERROR FileHandler::deleteFile(ID id){
     std::string file = this->getFile(id);
     return std::remove(file.c_str());
 }
 /**
  * @todo make threadsafe
  * @brief FileHandler::writeFile
  *  Write given text to an application tracked file
  * @param ID file id, std::string text
  * @return void
  */
 void FileHandler::writeFile(ID id, std::string text){
    std::string fileName = this->getFile(id);
    std::ofstream f (fileName.c_str(),std::ios::in);
    f << text.c_str();
 }

 /**
  * @brief FileHandler::readFile
  *  Read given lenght of lines to buffer from application
  *  tracked file. OBS! If number of lines exceeds =>
  *  reads to end of file (EOF)
  *  @param ID file id, std::string text
  *  @return voi
  */
 void FileHandler::readFile(ID id, size_t linesToRead, std::string& buf){
     std::ifstream f(this->getFile(id));
     while(linesToRead && std::getline(f,buf))
     {
         --linesToRead;
     }
 }
 /**
  * @brief FileHandler::getProject
  * Getter
  * @param ID project id
  * @return Project*
  */
 Project* FileHandler::getProject(ID pid){
     this->projMapLock.lock();
     Project* p = this->m_projects.at(pid);
     this->projMapLock.unlock();
     return p;
 }
 /**
  * @brief FileHandler::getFile
  * Getter
  * @param ID project file id
  * @return std::string filepath
  */
 std::string FileHandler::getFile(ID id){
    this->fileMapLock.lock();
    std::string file = this->m_fileMap.at(id);
    this->fileMapLock.unlock();
    return file;
 }
 /**
  * @brief FileHandler::getProject
  * Getter
  * @param ID directory id
  * @return directory path
  */
 std::string FileHandler::getDir(ID id){
    this->dirMapLock.lock();
    std::string dir = this->m_dirMap.at(id);
    this->dirMapLock.unlock();
    return dir;
 }

 /**
  * @brief FileHandler::addProject
  * Getter
  * @param std::pari<<ID, Project*> pair
  * @return void
  */
 void FileHandler::addProject(std::pair<ID,Project*> pair){
     this->dirMapLock.lock();
     this->m_projects.insert(pair);
     this->dirMapLock.unlock();

 }
 /**
  * @brief FileHandler::addFile
  * Getter
  * @param std::pari<<ID, std::string> pair
  * @return void
  */
void FileHandler::addFile(std::pair<ID,std::string> pair){
    this->fileMapLock.lock();
    this->m_fileMap.insert(pair);
    this->fileMapLock.unlock();
 }
 /**
  * @brief FileHandler::addDir
  * Getter
  * @param std::pari<<ID, std::string> pair
  * @return void
  */
void FileHandler::addDir(std::pair<ID,std::string> pair){
     this->dirMapLock.lock();
     this->m_dirMap.insert(pair);
     this->dirMapLock.unlock();
 }

