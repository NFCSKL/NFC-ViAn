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
    this->m_videos;
    this->m_dir = -1;

}
/**
* @brief Project::addVideo
* @param vid
* add given video to project
*/
void Project::add_video(Video vid)
{
    this->m_videos.push_back(vid);
}



