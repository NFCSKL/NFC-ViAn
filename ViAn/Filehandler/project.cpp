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
    this->saved = false;
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
    ps.projFile >> proj.m_name;

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
    ps.projFile << proj.m_name.c_str() << " ";
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
 * @brief operator <<
 * @param ps
 * @param pf
 * @return ps
 * Writes a projectfile object to projectstream
 * @deprecated
 * Shouldnt be needed, ids useless and filenames are standard.
 * isnt currently used but works as intended.
 * kept just in case.
 */
ProjectStream& operator<<(ProjectStream &ps,const ProjFiles& pf){
    ps.projFile << pf.f_analysis << " ";
    ps.projFile << pf.f_drawings << " ";
    ps.projFile << pf.f_videos << " ";
    return ps;

}

/**
 * @brief operator >>
 * @param ps
 * @param pf
 * @return ps
 * Reads files from a ProjFiles struct to a ProjectStream
 * Shouldnt be needed, ids useless and filenames are standard.
 * isnt currently used but works as intended.
 * kept just in case.
 */
ProjectStream& operator>>(ProjectStream &ps, ProjFiles& pf){
    std::string dummy;
    ps.projFile >> pf.f_analysis;
    ps.projFile >> pf.f_drawings;
    ps.projFile >> pf.f_videos;
    return ps;
}
