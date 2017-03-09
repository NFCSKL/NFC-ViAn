#include "project.h"
/**
 * @brief Project::Project
 * @param id
 * @param name
 */
Project::Project(ID id, std::string name)
{
    this->files = new ProjFiles();
    this->m_name = name;
    this->m_id = id;
    this->m_vid = 0;
    this->m_videos.clear();
}
/**
 * @brief Project::Project
 */
Project::Project(){
    this->files = new ProjFiles();
    this->m_name = "";
    this->m_id = -1;
    this->m_videos.clear();
}

/**
* @brief Project::add_video
* @param vid
* add given video to project
*/
void Project::add_video(Video* vid)
{
    this->m_videos.push_back(vid);
}
/**
 *  UNSFINISHED
 * @brief operator >>
 * @param is
 * @param proj
 * @return
 */
std::stringstream& operator>>(std::stringstream& is, Project& proj){
    int vidCounter = 0;
    std::string str;
    is >> str;
    while(vidCounter){
        Video* v = new Video();
        is >> *v;
        proj.add_video(v);
    }
    return is;
}
/**
 * @brief operator <<
 * @param os
 * @param proj
 * @return stream
 * used for reading project to file
 */
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
/**
 * @brief operator ==
 * @param proj
 * @param proj2
 * @return if projects are same TRUE else False
 */
bool operator==(Project proj, Project proj2){
   bool videoEquals =  std::equal(proj.m_videos.begin(), proj.m_videos.end(),
               proj2.m_videos.begin(),
               [](const Video* v, const Video* v2){return *v == *v2;});
    return proj.files == proj2.files &&
           proj.m_id == proj2.m_id &&
           proj.m_name == proj2.m_name &&
           videoEquals;

}
