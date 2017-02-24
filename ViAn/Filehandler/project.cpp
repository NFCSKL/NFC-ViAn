#include "project.h"

Project::Project(ID id, std::string name)
{
    this->m_name = name;
    this->m_id = id;
    this->m_vid = 0;
    this->m_videos;
    this->m_dir = -1;

}
//should we add Project
void Project::addVideo(Video vid)
{
    this->m_videos.push_back(vid);
}



