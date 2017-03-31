#include "project.h"
/**
 * @brief Project::Project
 * @param id
 * @param name
 */
Project::Project(ID id, std::string name)
{
    this->files = new ProjFiles();
    this->name = name;
    this->id = id;
    this->videos.clear();
    this->saved = false;
}
/**
 * @brief Project::Project
 */
Project::Project(){
    this->files = new ProjFiles();
    this->name = "";
    this->id = -1;
    this->videos.clear();
}

/**
* @brief Project::add_video
* @param vid
* add given video to project
*/
void Project::add_video(Video* vid)
{
    this->videos.push_back(vid);
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
    ps.proj_file >> proj.name;

    // read videos
    int vid_counter = 0;
    std::vector<Video*> temp; // used to preserve order ov videos, important for == operator
    ps.videos >> vid_counter;
    if( vid_counter < 0) return ps; // if negative number of videos, loop below will
                                   // be infinite. This is unlikely to happen. but just in case!
    while(vid_counter--){
        Video* v = new Video();
        ps.videos >> *v;
        temp.push_back(v);
    }
    for (auto vid_it = temp.rbegin(); vid_it < temp.rend(); ++vid_it) {  // to preserve order we add videos in reverse
        proj.add_video(*vid_it);
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
    ps.proj_file << proj.name.c_str() << " ";
    //write videos
    int vidcounter = proj.videos.size();
    ps.videos << vidcounter << " ";
    for(auto vid = proj.videos.rbegin(); vid != proj.videos.rend(); ++vid){
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
    ps.proj_file << pf.f_analysis << " ";
    ps.proj_file << pf.f_drawings << " ";
    ps.proj_file << pf.f_videos << " ";
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
    ps.proj_file >> pf.f_analysis;
    ps.proj_file >> pf.f_drawings;
    ps.proj_file >> pf.f_videos;
    return ps;
}
