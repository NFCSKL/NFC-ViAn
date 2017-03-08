#include "filehandler.h"

/**
 * @brief FileHandler::FileHandler
 */
FileHandler::FileHandler()
{
    this->m_pid = 0; // zero out counter ids
    this->m_fid = 0;
    this->m_did = 0;
    this->lastError = 0;

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
/**
 * @brief FileHandler::createDirectory
 * @param dirpath
 * @return unique directory ID
 */
ID FileHandler::createDirectory(std::string dirpath){
    this->lastError = makeDir(dirpath); //varying implementation, OS dependant
    ID id = this->addDir(dirpath);
    return id;
}

/**
 * @brief FileHandler::deleteDirectory
 * @param id
 * @return errorcode, if deletion was done code is 0;
 * otherwise see OS relevant directoryfile.
 */
FH_ERROR FileHandler::deleteDirectory(ID id){
    FH_ERROR err = removeDir(this->getDir(id)); //varying implementation, OS dependant
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
    proj->files->dir = dirID;

    proj->files->f_proj = createFile(fileName, dirID); //create project file


    fileName = proj->m_name + "_videos.txt";
    proj->files->f_videos = createFile(fileName, dirID); //create video file
    writeFile(proj->files->f_proj, fileName);

    fileName = proj->m_name + "_analyses.txt";
    proj->files->f_analysis = createFile(fileName, dirID); //create analysis file
    writeFile(proj->files->f_proj, fileName);

    fileName = proj->m_name + "_drawings.txt";
    proj->files->f_drawings =createFile(fileName, dirID); //create drawings file
    writeFile(proj->files->f_proj, fileName);

    updateProjFiles(proj);

}
/**
 * @todo unfinished, will be released with parser
 * however, is needed for creating
 * @brief FileHandler::saveProject
 * @param Project* name
 * Creates project and associated files.
 * @return void
 */
void FileHandler::updateProjFiles(Project* proj){
    std::string filePath = std::string(WORKSPACE) + "/" + proj->m_name + "/" + proj->m_name + std::string("_videos.txt");
    std::stringstream sstr;
    sstr << *proj;
    writeFile(proj->files->f_videos, sstr.str());
}

/**
 * @todo unfinished, will be released with parser, needs full
 * project structure and file to program parser to finish.
 * @brief FileHandler::loadProject
 * @param std::string, fileopath to project file
 * Load a project object from a given filepath
 */
Project* FileHandler::loadProject(std::string name, std::string dirpath){
    Project* proj = new Project();
    std::ifstream f(dirpath + "/" + name + ".txt");
    std::string filename;
    std::getline(f, filename);
    proj->files->f_videos = addFile(dirpath + "/" +filename);
    std::stringstream sstr;
    std::string buf = "";
    readFile(proj->files->f_videos, buf, 1);
    sstr << buf;
    std::cout << "**sstr**" << sstr.str() << std::endl;
    sstr >> *proj;
    std::stringstream check;
    check << *proj;
    std::cout << "**check**" << check.str() << std::endl;
    return proj;
}

/**
 * @brief FileHandler::deleteProject
 * Deletes project, its associated files and contents.
 * OBS! This operation is as of now irreversible
 * @param Project*
 * @return int errorcode
 */
FH_ERROR FileHandler::deleteProject(Project* proj){
    ProjFiles* pf = proj->files;
    deleteFile(pf->f_proj);
    deleteFile(pf->f_videos);
    deleteFile(pf->f_analysis);
    deleteFile(pf->f_drawings);
    return deleteDirectory(proj->files->dir);

}
/**
 * @todo make threadsafe
 * @brief FileHandler::addVideo
 * @param Project*, std::string
 * Add a video filepath to a given project.
 * Creates Video object which is accessed further by returned id.
 */
void FileHandler::addVideo(Project* proj, std::string filePath){
    Video* v = new Video(this->m_fid, filePath);
    proj->addVideo(v);
    this->addFile(filePath);
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
    return this->addFile(filePath);

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
    std::ofstream f (fileName.c_str(), std::ios::in | std::ios::out | std::ios::ate);
    f << text.c_str() << std::endl;
 }

 /**
  * @brief FileHandler::readFile
  *  Read given lenght of lines to buffer from application
  *  tracked file. OBS! If number of lines exceeds =>
  *  reads to end of file (EOF)
  *  @param ID file id, std::string text
  *  @return voi
  */
 void FileHandler::readFile(ID id,  std::string& buf, size_t linesToRead){
     std::ifstream f(this->getFile(id));
     if(f.is_open())
        while(linesToRead-- && std::getline(f, buf));
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
    this->projMapLock.lock();
    this->m_projects.insert(pair);
    this->projMapLock.unlock();

 }
 /**
  * @brief FileHandler::addFile
  * Getter
  * @param std::pari<<ID, std::string> pair
  * @return void
  */
ID FileHandler::addFile(std::string filepath){
    std::pair<ID,std::string> pair = std::make_pair(this->m_fid, filepath);
    this->fileMapLock.lock();
    this->m_fileMap.insert(pair);
    this->fileMapLock.unlock();
    return this->m_fid++;
 }
 /**
  * @brief FileHandler::addDir
  * Getter
  * @param std::pari<<ID, std::string> pair
  * @return void
  */
ID FileHandler::addDir(std::string dirpath){
    std::pair<ID,std::string> pair = std::make_pair(this->m_did, dirpath);
    this->dirMapLock.lock();
    this->m_dirMap.insert(pair);
    this->dirMapLock.unlock();
    return this->m_did++;
 }

