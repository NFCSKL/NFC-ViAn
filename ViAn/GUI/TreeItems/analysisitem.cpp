#include "analysisitem.h"


AnalysisItem::AnalysisItem(AnalysisProxy* analysis) : TreeItem(ANALYSIS_ITEM) {
    m_analysis = analysis;
    finished = true;
    const QIcon folder_icon("../ViAn/Icons/analysis.png");
    setIcon(0, folder_icon);
    setText(0, QString::fromStdString(m_analysis->get_name()));
}

AnalysisItem::AnalysisItem() : TreeItem(ANALYSIS_ITEM) {
    const QIcon folder_icon("../ViAn/Icons/analysis.png");
    setIcon(0, folder_icon);
    setText(0, "Analysis");
}

AnalysisItem::~AnalysisItem() {}

void AnalysisItem::set_analysis(AnalysisProxy *analysis) {
    m_analysis = analysis;
    finished = true;
}

bool AnalysisItem::is_finished() const {
    return finished;
}

AnalysisProxy* AnalysisItem::get_analysis() {
    return m_analysis;
}

void AnalysisItem::remove(){}

void AnalysisItem::rename(){
//    m_analysis->set_name(text(0).toStdString());
}
