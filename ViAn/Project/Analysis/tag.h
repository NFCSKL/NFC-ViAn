#ifndef TAG_H
#define TAG_H
#include "basicanalysis.h"
class BasicAnalysis;

class Tag : public BasicAnalysis {
    std::set<int> m_frames;
    bool drawing_tag = false;

public:
    virtual ANALYSIS_TYPE get_type() const override;
    virtual void read(const QJsonObject& json) override;
    virtual void write(QJsonObject &json) override;
    bool add_frame(int);
    bool remove_frame(int);
    std::set<int> get_frames();
    bool is_drawing_tag();
    void set_drawing_tag(bool);
};

#endif // TAG_H
