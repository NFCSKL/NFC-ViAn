#include "project.h"
/**
 * @brief Project::Project
 * @param id
 * @param name
 */
Project::Project(ID id, std::string name)
{
    this->m_name = name;
    this->m_id = id;
    this->m_vid = 0;
    this->m_videos.clear();
    this->m_dir = -1;

}
Project::Project(){
    this->m_name = "";
    this->m_id = -1;
    this->m_vid = 0;
    this->m_videos.clear();
    this->m_dir = -1;
}

/**
* @brief Project::addVideo
* @param vid
* add given video to project
*/
void Project::addVideo(Video* vid)
{
    this->m_videos.push_back(vid);
}

std::stringstream& operator>>(std::stringstream& is, Project& proj){
    int vidCounter = 0;
    std::string str;
    is >> str;
    std::cout << "*****";
    std::cout <<str;
    std::cout << "*****";
    while(vidCounter){
        Video* v = new Video();
        is >> *v;
        proj.addVideo(v);
    }
    return is;
}

std::stringstream& operator<<(std::stringstream& os, const Project& proj){
    os << proj.m_id << " " << proj.m_name << " ";
    int vidcounter = 0;

    for(auto vid = proj.m_videos.rbegin(); vid != proj.m_videos.rend(); ++vid){
        Video* v = *vid;
        os << *v << " ";
        vidcounter++;
    }
    os << vidcounter;
    return os;
}
bool operator==(Project proj, Project proj2){
   bool videoEquals =  std::equal(proj.m_videos.begin(), proj.m_videos.end(),
               proj2.m_videos.begin(),
               [](const Video* v, const Video* v2){return *v == *v2;});
    return proj.m_dir == proj2.m_dir &&
           proj.m_file == proj2.m_file &&
           proj.m_id == proj2.m_id &&
           proj.m_name == proj2.m_name &&
           videoEquals;

}
