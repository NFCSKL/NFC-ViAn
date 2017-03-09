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
* @brief Project::addVideo
* @param vid
* add given video to project
*/
void Project::addVideo(Video* vid)
{
    this->m_videos.push_back(vid);
}
/**
 *  UNSFINISHED
 * @brief operator >>
 * @param is
 * @param proj
 * @return stringstream containing project information
 */
std::stringstream& operator>>(std::stringstream& is, Project& proj){
    int vidCounter = 0;
    std::vector<Video*> temp; // used to preserve order ov videos, important for == operator
    is >> proj.m_id ;
    is >> proj.m_name ;
    is >> vidCounter;
    if( vidCounter < 0) return is; // if negative number of videos, loop below will
                                   // be infinite. This is unlikely to happen. but just in case!
    while(vidCounter--){
        Video* v = new Video();
        is >> *v;
        temp.push_back(v);
    }
    for (auto vidIt = temp.rbegin(); vidIt < temp.rend(); ++vidIt) {  // to preserve order we add videos in reverse
        proj.addVideo(*vidIt);
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
    int vidcounter = proj.m_videos.size();
    os << vidcounter << " ";
    for(auto vid = proj.m_videos.rbegin(); vid != proj.m_videos.rend(); ++vid){
        Video* v = *vid;
        os << *v << " ";
        vidcounter++;
    }
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
               [](const Video* v, const Video* v2){return *v == *v2;}); // lambda function comparing using video==
                                                                        // by dereferencing pointers in vector
    return //*proj.files == *proj2.files && probably unnecessary as projfiles have projname followed by default suffix
           proj.m_name == proj2.m_name &&
           videoEquals;

}
/**
 * @brief operator ==
 * @param pf
 * @param pf2
 * @retur
 * may not be needed but works as intended,
 */
bool operator==(ProjFiles pf, ProjFiles pf2){
    std::cout << "bool: " << (pf.dir == pf2.dir ) << std::endl;
    std::cout << "bool: " << (pf.f_analysis == pf2.f_analysis)   << std::endl;
    std::cout << "bool: " << (pf.f_drawings == pf2.f_drawings  )<< std::endl;
    std::cout << "bool: " <<  (pf.f_videos == pf2.f_videos) << std::endl;
    return pf.dir == pf2.dir &&
            pf.f_analysis == pf2.f_analysis &&
            pf.f_proj == pf2.f_proj &&
            pf.f_videos == pf2.f_videos;
}
