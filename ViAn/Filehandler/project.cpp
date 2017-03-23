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
 * @return stringstream containing project information
 */
ProjectStream& operator>>(ProjectStream& ps, Project& proj){
    //write files
    //Read project id and name
    ps.projFile >> proj.m_id;
    ps.projFile >> proj.m_name;
    ps >> *(proj.files);

    // read videos
    int vidCounter = 0;
    std::vector<Video*> temp; // used to preserve order ov videos, important for == operator
    ps.videos >> vidCounter;
    if( vidCounter < 0) return ps; // if negative number of videos, loop below will
                                   // be infinite. This is unlikely to happen. but just in case!
    while(vidCounter--){
        Video* v = new Video();
        ps.videos >> *v;
        temp.push_back(v);
    }
    for (auto vidIt = temp.rbegin(); vidIt < temp.rend(); ++vidIt) {  // to preserve order we add videos in reverse
        proj.add_video(*vidIt);
    }
    return ps;
}
/**
 * @brief operator <<
 * @param os
 * @param proj
 * @return stream
 * used for writing videos to file
 */
ProjectStream& operator<<(ProjectStream &ps, const Project& proj){
    //write name and id;   
    ps.projFile << proj.m_id<< " ";
    ps.projFile << proj.m_name.c_str() << " ";
    //write files
    ps << *(proj.files);
    //write videos
    int vidcounter = proj.m_videos.size();
    ps.videos << vidcounter << " ";
    for(auto vid = proj.m_videos.rbegin(); vid != proj.m_videos.rend(); ++vid){
        Video* v = *vid;
        ps.videos << *v << " ";
        vidcounter++;
    }
    return ps;
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
    return *proj.files == *proj2.files && //probably unnecessary as projfiles have projname followed by default suffix
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
    return  pf.dir == pf2.dir &&
            // Not used in current implementation
            pf.f_analysis == pf2.f_analysis &&
            pf.f_proj == pf2.f_proj &&
            pf.f_videos == pf2.f_videos;
}
ProjectStream& operator<<(ProjectStream &ps,const ProjFiles& pf){
    ps.projFile << pf.f_proj << " ";
    ps.projFile << pf.dir << " ";
    ps.projFile << pf.f_analysis << " ";
    ps.projFile << pf.f_drawings << " ";
    ps.projFile << pf.f_videos << " ";
    return ps;

}
ProjectStream& operator>>(ProjectStream &ps, ProjFiles& pf){
    std::string dummy;

    ps.projFile >> pf.f_proj;
    ps.projFile >> pf.dir;
    ps.projFile >> pf.f_analysis;
    ps.projFile >> pf.f_drawings;
    ps.projFile >> pf.f_videos;
    return ps;
}
