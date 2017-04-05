#include "videoproject.h"

VideoProject::VideoProject(Video* v){
    this->video = v;
}
Video* VideoProject::get_video(){
    return this->video;
}
