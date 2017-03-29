#include "filehandler.h"

/**
 * @brief FileHandler::FileHandler
 */
FileHandler::FileHandler() {
    this->m_pid = 0; // zero out counter ids
    this->m_fid = 0;
    this->m_did = 0;
    this->lastError = 0;

}

/**
 * @brief FileHandler::create_project
 * creates project and associated files.
 * @param std::string name
 * @return Project* created project
 */
Project* FileHandler::create_project(std::string projName){
    Project* proj =  new Project(this->m_pid, projName);
    this->m_projects.insert(std::make_pair(proj->m_id, proj));
    save_project(proj);
    this->m_pid++;
    return proj;
}

/**
 * @brief FileHandler::create_directory
 * @param dirpath
 * @return unique directory ID
 */
ID FileHandler::create_directory(std::string dirpath){
    this->lastError = make_dir(dirpath); //varying implementation, OS dependant
    ID id = this->add_dir(dirpath);
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
    save_project(get_project(id));
}

/**
 * @todo unfinished, needs full project structure
 * and program to file parser to finish
 * @brief FileHandler::save_project
 * @param Project* name
 * Creates project and associated files.
 * @return void
 */
void FileHandler::save_project(Project* proj){
    std::string projFile = proj->m_name + std::string(".txt"); //filename
    if(!proj->saved){
        ID dirID = create_directory(std::string(WORKSPACE) + "/"+ proj->m_name);//project directory

        proj->files->dir = dirID;

        proj->files->f_proj = create_file(projFile, dirID); //create project file

        std::string vidFile = proj->m_name + "_videos.txt";
        proj->files->f_videos = create_file(vidFile, dirID); //create video file


        std::string analysisFile = proj->m_name + "_analyses.txt";
        proj->files->f_analysis = create_file(analysisFile, dirID); //create analysis file


        std::string drawingFile = proj->m_name + "_drawings.txt";
        proj->files->f_drawings =create_file(drawingFile, dirID); //create drawings file
    }
    update_proj_files(proj);

}

/**
 * @todo unfinished, will be released with parser
 * however, is needed for creating
 * @brief FileHandler::save_project
 * @param Project* name
 * Creates project and associated files.
 * @return void
 */
void FileHandler::update_proj_files(Project* proj){
    ProjectStream ps;
    ps << *proj;
    write_file(proj->files->f_proj, ps.projFile.str(), WRITE_OPTION::OVERWRITE);
    write_file(proj->files->f_videos, ps.videos.str(), WRITE_OPTION::OVERWRITE);
    write_file(proj->files->f_analysis, ps.analyzes.str(), WRITE_OPTION::OVERWRITE);
    write_file(proj->files->f_drawings, ps.drawings.str(), WRITE_OPTION::OVERWRITE);
}
void FileHandler::load_proj_files(std::string str){
    ID id;
    std::string filepath;
    std::stringstream sstr;
    sstr << str;    
    //read files until empty
    while(sstr >> id >> filepath){
        add_file(id, filepath);
    }
}

/**
 * @brief FileHandler::load_project
 * @param dirpath
 * @return
 */
Project* FileHandler::load_project(std::string fullProjectPath){
    std::string dirpath = fullProjectPath.substr(0, fullProjectPath.find_last_of("/"));
    std::string proj_name = fullProjectPath.substr(fullProjectPath.find_last_of("/")+1, fullProjectPath.length());
    proj_name = proj_name.substr(0, proj_name.find(".txt"));
    return load_project(proj_name, dirpath);
}

/**
 * @todo load analyses (in project <</>> operators)
 * @todo load drawings (in project <</>> operators)
 * @brief FileHandler::load_project
 * @param projname
 * @param dirpath
 * @return project
 */
Project* FileHandler::load_project(std::string projname, std::string dirpath){
    Project* proj = new Project();
    proj->m_id = this->m_pid;
    proj->files->dir = add_dir(dirpath);

    add_project(std::make_pair(this->m_pid++, proj));
    ProjectStream projStream;

    proj->saved = true;
//    Read project file
    std::string projFilePath = dirpath + "/" + projname + ".txt";
    proj->files->f_proj = load_project_file(projFilePath, projStream.projFile);

//    Read video file
    std::string videoFilePath = dirpath + "/" + projname + "_videos.txt";
    proj->files->f_videos = load_project_file(videoFilePath, projStream.videos);
//    Read Analyzes
    std::string analysesFilePath = dirpath + "/" + projname + "_analyses.txt";
    proj->files->f_analysis = load_project_file(analysesFilePath, projStream.videos);
//    Read Drawings
    std::string drawingsFilePath = dirpath + "/" + projname + "_drawings.txt";
    proj->files->f_drawings = load_project_file(drawingsFilePath, projStream.drawings);
//    Read project from projstream
    projStream >> *proj;
    return proj;
}

/**
 * @brief FileHandler::load_project_file
 * help function for load_project
 * @param filePath
 * @param projFileStream
 * @return ID
 */
ID FileHandler::load_project_file(std::string filePath, std::stringstream& projFileStream){
    std::string buf;
    ID projFileID = add_file(filePath);
    read_file(projFileID, buf);
    projFileStream << buf; // Read project name
    return projFileID;
}

/**
 * @brief FileHandler::delete_project
 * Deletes project, its associated files and contents.
 * OBS! This operation is as of now irreversible
 * @param Project*
 * @return FH_ERROR errorcode
 */
FH_ERROR FileHandler::delete_project(Project* proj){
    ProjFiles* pf = proj->files;
    delete_file(pf->f_proj);
    delete_file(pf->f_videos);
    delete_file(pf->f_analysis);
    delete_file(pf->f_drawings);
    return delete_directory(proj->files->dir);

}
/**
 * @todo make threadsafe
 * @brief FileHandler::add_video
 * @param Project*,string filepath
 *
 * string
 * Add a video filepath to a given project.
 * Creates Video object which is accessed further by returned id.
 */
ID FileHandler::add_video(Project* proj, std::string filePath){
    Video* v = new Video(filePath);
    proj->add_video(v);
    return this->add_file(filePath);
}

void FileHandler::remove_video(ID proj_id, ID vid_id){
    Project* proj = this->get_project(proj_id);
    proj->remove_video(vid_id);
}
 /**
  * @brief FileHandler::create_file
  * create a file by given name in already excisting
  * application tracked directory
  * @param std::string file name, ID directory id
  */

ID FileHandler::create_file(std::string filename, ID dirID){
    std::ofstream f;
    std::string filePath = this->get_dir(dirID)+"/"+filename;
    f.open(filePath.c_str());
    return this->add_file(filePath);
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
    std::string fileName = this->get_file(id);
    std::ofstream f;
    switch(opt){
    case WRITE_OPTION::OVERWRITE:
        f.open(fileName.c_str(), std::ios::in | std::ios::out);
        break;
    case WRITE_OPTION::APPEND:
        f.open(fileName.c_str(), std::ios::in | std::ios::out | std::ios::ate);
        break;
    default:
        return; // no open file
        break;
    }
    if(f.is_open()) f << text.c_str() << std::endl;
 }

 /**
  * @brief FileHandler::read_file
  *  Read given lenght of lines to buffer from application
  *  tracked file. OBS! If number of lines exceeds =>
  *  reads to end of file (EOF)
  *  @param ID file id, std::string text
  *  @return voi
  */
 void FileHandler::read_file(ID id,  std::string& buf, int linesToRead){
     std::ifstream f(this->get_file(id));
     std::string temp;
     if(f.is_open()){
         while(linesToRead-- && std::getline(f, temp)){
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
 Project* FileHandler::get_project(ID pid){
    this->projMapLock.lock();
    Project* p = this->m_projects.at(pid);
    this->projMapLock.unlock();
    return p;
 }

 /**
  * @brief FileHandler::get_file
  * Getter
  * @param ID project file id
  * @return std::string filepath
  */
 std::string FileHandler::get_file(ID id){
    this->fileMapLock.lock();
    std::string file = this->m_fileMap.at(id);
    this->fileMapLock.unlock();
    return file;
 }

 /**
  * @brief FileHandler::get_dir
  * @param ID directory id
  * @return directory path
  */
 std::string FileHandler::get_dir(ID id){
    this->dirMapLock.lock();
    std::string dir = this->m_dirMap.at(id);
    this->dirMapLock.unlock();
    return dir;
 }

 /**
  * @brief FileHandler::add_projectr
  * @param std::pari<<ID, Project*> pair
  * @return void
  */
 void FileHandler::add_project(std::pair<ID,Project*> pair){
    this->projMapLock.lock();
    this->m_projects.insert(pair);
    this->projMapLock.unlock();

 }

 /**
  * @brief FileHandler::add_file
  * @param std::string filepath
  * @return unique file identifier
  */
ID FileHandler::add_file(std::string filepath){
    add_file(this->m_fid, filepath);
    return this->m_fid++;
 }

/**
 * @brief FileHandler::add_file
 * @param id
 * @param filepath
 */
void FileHandler::add_file(ID id ,std::string filepath){
    std::pair<ID,std::string> pair = std::make_pair(id, filepath);
    this->fileMapLock.lock();
    this->m_fileMap.insert(pair);
    this->fileMapLock.unlock();
}

 /**
  * @brief FileHandler::add_dir
  * @param std::string dirpath
  * @return unique directory identifier
  */
ID FileHandler::add_dir(std::string dirpath){
    std::pair<ID,std::string> pair = std::make_pair(this->m_did, dirpath);
    this->dirMapLock.lock();
    this->m_dirMap.insert(pair);
    this->dirMapLock.unlock();
    return this->m_did++;
 }

/**
 * @brief FileHandler::proj_equals
 * @param proj
 * @param proj2
 * @return true if project contents are the same
 */
bool FileHandler::proj_equals(Project& proj, Project& proj2){
    bool videoEquals =  std::equal(proj.videos.begin(), proj.videos.end(),
               proj2.videos.begin(),
               [](const std::pair<ID,Video*> v, const std::pair<ID,Video*> v2){return v.first == v2.first &&
                                                                               *(v.second) == *(v2.second);}); // lambda function comparing using video==
                                                                        // by dereferencing pointers in vector
    return projfiles_equal(*proj.files , *proj2.files) && //probably unnecessary as projfiles have projname followed by default suffix
           proj.m_name == proj2.m_name &&
           videoEquals;
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
