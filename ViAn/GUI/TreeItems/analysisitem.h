#ifndef ANALYSISITEM_H
#define ANALYSISITEM_H

#include "treeitem.h"

class AnalysisProxy;
class AnalysisItem : public TreeItem {
    AnalysisProxy* m_analysis = nullptr;
    bool finished = false;
    bool is_new = false;
public:
    AnalysisItem(AnalysisProxy* analysis);
    AnalysisItem(int type);
    ~AnalysisItem();
    void set_analysis(AnalysisProxy* analysis);
    AnalysisProxy *get_analysis();
    bool is_finished() const;
    void remove();
    void rename();
    void set_not_new();

    bool saved = true;
};
#endif // ANALYSISITEM_H
