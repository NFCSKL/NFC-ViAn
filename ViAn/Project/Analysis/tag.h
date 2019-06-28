#ifndef TAG_H
#define TAG_H

#include "basicanalysis.h"

#include <QJsonObject>

#include <map>
#include <vector>

class TagFrame;

class Tag : public BasicAnalysis {
    int m_type = TAG;

public:
    Tag(QString name = "", int type = TAG);
    ~Tag() override;
    virtual int get_type() const override;
    virtual void read(const QJsonObject& json) override;
    virtual void write(QJsonObject &json) override;
    void add_frame(int frame, TagFrame *t_frame);
    bool find_frame(int);
    void remove_frame(int);
    void update_color_correction(int frame, int b_value, double c_value, double g_value);
    void update_color_whole_tag(int b, double c, double g);
    void update_index_tag();
    int next_frame(int);
    int previous_frame(int);
    std::vector<int> get_frames();
    bool is_drawing_tag();
    void revert_tag_map();

    std::map<int, TagFrame*> tag_map;
    std::map<int, TagFrame*> saved_map;
};

#endif // TAG_H
