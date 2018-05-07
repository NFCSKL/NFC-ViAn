#ifndef ANALYSISITEM_H
#define ANALYSISITEM_H
#include "treeitem.h"
#include "Project/Analysis/analysisproxy.h"
class AnalysisItem : public TreeItem {
    AnalysisProxy* m_analysis = nullptr;
    bool finished = false;
    bool is_new = false;
public:
    AnalysisItem(AnalysisProxy* analysis);
    AnalysisItem();
    ~AnalysisItem();
    void set_analysis(AnalysisProxy* analysis);
    AnalysisProxy *get_analysis();
    bool is_finished() const;
    void remove();
    void rename();
    void set_not_new();

    const QString NEW_STR = " - NEW!";
};
#endif // ANALYSISITEM_H
