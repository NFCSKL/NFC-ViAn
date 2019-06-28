#ifndef TAGFRAMEITEM_H
#define TAGFRAMEITEM_H

#include "Project/video.h"
#include "treeitem.h"

class TagFrame;
class TagFrameItem : public TreeItem {
    int m_frame;
    QString m_name;
    TagFrame* m_t_frame = nullptr;
public:
    TagFrameItem(int frame);
    TagFrameItem(TagFrame *t_frame);
    VideoState get_state();
    int get_frame();
    void remove();
    void rename();
};

#endif // TAGFRAMEITEM_H
