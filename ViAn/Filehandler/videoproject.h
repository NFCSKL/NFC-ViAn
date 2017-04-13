#ifndef VIDEOPROJECT_H
#define VIDEOPROJECT_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "analysis.h"
enum ANALYSIS_TYPE{MOVEMENT}; // Analyses have different types
/**
 * @todo change map to include ID for flexibility.. I.E allow multiple analyses of each type.
 * @brief The VideoProject class
 * Class for storing video and all its belonging components
 * such as analyses, drawings and documentation.
 */
class VideoProject{
    std::map<ANALYSIS_TYPE, Analysis> analyses; // Only one analysis of each type
    Video* video;
public:

    VideoProject(Video* v); //Needs to have a video
    Video* get_video();

};


#endif // VIDEOPROJECT_H
