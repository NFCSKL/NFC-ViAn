#ifndef TAG_H
#define TAG_H
#include "basicanalysis.h"
#include "Project/video.h"
#include "tagframe.h"
#include <map>

class BasicAnalysis;
class Video;

class Tag : public BasicAnalysis {
    bool m_drawing_tag = false;

public:
    Tag(std::string name = "", bool drawing_tag = false);
    ~Tag() override;
    virtual ANALYSIS_TYPE get_type() const override;
    virtual void read(const QJsonObject& json) override;
    virtual void write(QJsonObject &json) override;
    void add_frame(int frame, TagFrame *t_frame);
    bool find_frame(int);
    void remove_frame(int);
    void update_color_correction(int frame, int b_value, double c_value);
    int next_frame(int);
    int previous_frame(int);
    std::vector<int> get_frames();
    bool is_drawing_tag();
    void set_drawing_tag(bool);
    void update_color_whole_tag(int b, double c);

    std::map<int, TagFrame*> tag_map;
};

#endif // TAG_H
