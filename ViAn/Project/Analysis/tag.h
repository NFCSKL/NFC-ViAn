#ifndef TAG_H
#define TAG_H
#include "basicanalysis.h"
#include "Project/video.h"
class BasicAnalysis;
class Video;

class Tag : public BasicAnalysis {
    std::set<int> m_frames;
    bool drawing_tag = false;
    std::map<int, VideoState> new_tags;

public:
    ~Tag() override;
    virtual ANALYSIS_TYPE get_type() const override;
    virtual void read(const QJsonObject& json) override;
    virtual void write(QJsonObject &json) override;
    bool add_frame(int);
    void add_frame_state(int frame, VideoState state);
    bool remove_frame(int);
    int next_frame(int);
    int previous_frame(int);
    std::set<int> get_frames();
    bool is_drawing_tag();
    void set_drawing_tag(bool);
};

#endif // TAG_H
