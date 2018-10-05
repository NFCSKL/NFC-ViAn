#include "analysisitem.h"
/**
 * @brief AnalysisItem::AnalysisItem
 * @param analysis
 */
AnalysisItem::AnalysisItem(AnalysisProxy* analysis) : TreeItem(ANALYSIS_ITEM) {
    m_analysis = analysis;
    finished = true;
    const QIcon folder_icon("../ViAn/Icons/analysis.png");
    setIcon(0, folder_icon);
    setText(0, QString::fromStdString(m_analysis->get_name()));
}
/**
 * @brief AnalysisItem::AnalysisItem
 */
AnalysisItem::AnalysisItem() : TreeItem(ANALYSIS_ITEM) {
    const QIcon folder_icon("../ViAn/Icons/analysis.png");
    setIcon(0, folder_icon);
    setText(0, "Analysis");
}
/**
 * @brief AnalysisItem::~AnalysisItem
 */
AnalysisItem::~AnalysisItem() {}

/**
 * @brief AnalysisItem::set_analysis
 * @param analysis
 */
void AnalysisItem::set_analysis(AnalysisProxy *analysis) {
    m_analysis = analysis;
    finished = true;
    is_new = true;
    setText(0, text(0) + NEW_STR);
    setBackgroundColor(0, "#FFFDAB");
}

/**
 * @brief AnalysisItem::is_finished
 * @return
 */
bool AnalysisItem::is_finished() const {
    return finished;
}

/**
 * @brief AnalysisItem::get_analysis
 * @return
 */
AnalysisProxy* AnalysisItem::get_analysis() {
    return m_analysis;
}

void AnalysisItem::remove(){}

/**
 * @brief AnalysisItem::rename
 * Rename item
 */
void AnalysisItem::rename(){
    if (is_new) {
        m_analysis->m_name = text(0).remove(NEW_STR).toStdString();
    } else {
        m_analysis->m_name = text(0).toStdString();
    }
}

void AnalysisItem::set_not_new() {
    if (is_new) {
        setText(0, text(0).remove(NEW_STR));
        setBackgroundColor(0, Qt::white);
        is_new = false;
    }
}
