#include "filehandler.h"

FileHandler::FileHandler()
{
    this->m_pid = 0;

}

////create a new project given a working directot
Project* FileHandler::createProject(std::string projName){
    Project* proj =  new Project(this->m_pid, projName);
    this->m_projects.insert(std::make_pair(proj->m_id, proj));
    saveProject(proj);
    this->m_pid++;
    return proj;
}
//Read from project file and build Project object

void FileHandler::saveProject(Project* proj){
    std::string fileName = proj->m_name + std::string(".txt");
    ID dirID = createDirectory(std::string(WORKSPACE) + "/"+ proj->m_name);
    //TODO : extend seperation of created and existing.
    // directory created
    proj->m_dir = dirID;
    proj->m_file = createFile(fileName, dirID);
    //write project info to file
    std::string projstr = "";
    projstr += "Name:"+proj->m_name + "\n";
    std::vector<Video> vids = proj->m_videos;
    for (std::vector<Video>::iterator v = vids.begin();v != vids.end(); ++v) {
        projstr += "V" + std::to_string(v->id) + ":" + this->m_fileMap.at(v->id) + "\n";
    }
    writeFile(proj->m_file, projstr);


}

int FileHandler::deleteProject(Project* proj){
    createDirectory(this->m_dirMap.at(proj->m_dir));
    if (GetLastError() == ERROR_ALREADY_EXISTS){
        deleteFile(proj->m_file);
        deleteDirectory(this->m_dirMap.at(proj->m_dir));
        return 0;
    }
    else{
        return -1;
    }
}
void FileHandler::addVideo(Project* proj, std::string filePath){
    Video v (this->m_fid);
    proj->addVideo(v);
    this->m_fileMap.insert(std::make_pair(this->m_fid++,filePath));
}

Project* FileHandler::loadProject(std::string filePath){
    Project* proj;
    std::ifstream f(filePath);
    std::string content = "";
    std::string line = "";
    std::getline(f,line);
    //proj = createProject(line.substr(0, line.find(":")));
    while(std::getline(f,line))
    {
        extract_proj_obj(line);
    }
}

 void FileHandler::extract_proj_obj(std::string line){

    char c = line[0];
    if(c == 'V') // Analysis
        line.substr(0, line.find(":"));
    if(c == 'A')
        line.substr(0, line.find(":"));
    if(c == 'D')
        line.substr(0, line.find(":"));

 }

 ID FileHandler::createDirectory(std::string dirpath){
     CreateDirectory(s2ws(dirpath).c_str(),NULL);
     this->m_dirMap.insert(std::make_pair(this->m_did, dirpath));
     return this->m_did++;
 }

 int FileHandler::deleteDirectory(std::string dirpath){
     RemoveDirectory(s2ws(dirpath).c_str());    
     return GetLastError();
 }
 ID FileHandler::createFile(std::string filename, ID dirID){
     std::ofstream f;
     std::string filePath = this->m_dirMap.at(dirID)+"/"+filename;
     f.open(filePath.c_str());
     f.close();
     this->m_fileMap.insert(std::make_pair(this->m_fid, filePath));
     return this->m_fid++;
  }

 int FileHandler::deleteFile(ID id){
     std::string file = this->m_fileMap.at(id);
     return std::remove(file.c_str());
 }

 void FileHandler::writeFile(ID id, std::string text){

    std::string fileName = this->getFile(id);
    std::ofstream f (fileName.c_str(),std::ios::in);
    std::cout << "file to write to: "<< fileName.c_str() <<std::endl<< "texttowrite\n"<<text;
    f << text.c_str();
    f.close();
 }

 void FileHandler::readFile(ID id, size_t linesToRead, std::string& buf){
     std::ifstream f(this->getFile(id));
     while(linesToRead && std::getline(f,buf))
     {
         --linesToRead;
     }
 }

 Project* FileHandler::getProject(ID pid){
     return this->m_projects.at(pid);
 }
 std::string FileHandler::getFile(ID id){
    return this->m_fileMap.at(id);
 }
 std::string FileHandler::getDir(ID id){
     return this->m_dirMap.at(id);
 }
