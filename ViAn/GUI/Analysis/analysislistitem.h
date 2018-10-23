#ifndef ANALYSISLISTITEM_H
#define ANALYSISLISTITEM_H

#include <QListWidgetItem>

class AnalysisMethod;

/**
 * @brief The AnalysisListItem class
 * AnalysisItem wrapper for analysismethod
 * in QListWidget
 */
class AnalysisListItem : public QListWidgetItem {

public:
    AnalysisMethod* m_analysis;
    AnalysisListItem(AnalysisMethod* method);
};

#endif // ANALYSISLISTITEM_H
