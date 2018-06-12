#ifndef TAGFRAME_H
#define TAGFRAME_H

#include "Project/video.h"


class TagFrame {

public:
    TagFrame(int frame);
    TagFrame(int frame, VideoState state);
    ~TagFrame();

    VideoState m_state;
    int m_frame;
};

#endif // TAGFRAME_H
