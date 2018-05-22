#ifndef TAG_H
#define TAG_H
#include "basicanalysis.h"
#include "Project/video.h"
#include "tagframe.h"
#include <map>

class BasicAnalysis;
class Video;

class Tag : public BasicAnalysis {
    bool drawing_tag = false;
    std::map<int, TagFrame*> tag_map;


public:
    ~Tag() override;
    virtual ANALYSIS_TYPE get_type() const override;
    virtual void read(const QJsonObject& json) override;
    virtual void write(QJsonObject &json) override;
    void add_frame(int frame, TagFrame *t_frame);
    bool find_frame(int);
    void remove_frame(int);
    int next_frame(int);
    int previous_frame(int);
    std::map<int, TagFrame *> get_frames();
    bool is_drawing_tag();
    void set_drawing_tag(bool);
};

#endif // TAG_H
