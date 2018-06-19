#ifndef TAGFRAMEITEM_H
#define TAGFRAMEITEM_H
#include "treeitem.h"
#include "Project/video.h"
#include "tagitem.h"

class TagFrameItem : public TreeItem {
    int m_frame;
    TagFrame* m_t_frame = nullptr;
public:
    TagFrameItem(int frame);
    void set_state(TagFrame *t_frame);
    VideoState get_state();
    int get_frame();
    void remove();
    void rename();
};

#endif // TAGFRAMEITEM_H
