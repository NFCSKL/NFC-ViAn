#include "analysislistitem.h"

/**
 * @brief AnalysisListItem::AnalysisListItem
 * @param method
 * Set method and text
 */
AnalysisListItem::AnalysisListItem(AnalysisMethod *method) {
    m_analysis = method;
    setText("method");
}
