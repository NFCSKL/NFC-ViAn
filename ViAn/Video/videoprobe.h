#ifndef VIDEOPROBE_H
#define VIDEOPROBE_H

#include <opencv2/core/core.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/video/video.hpp>


class VideoProbe
{
public:
    VideoProbe();

    static run(const std::string& video_path);
};

#endif // VIDEOPROBE_H
