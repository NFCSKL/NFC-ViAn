#ifndef ANALYSISCONTROLLER_H
#define ANALYSISCONTROLLER_H

#include <QThread>
#include "Filehandler/analysis.h"
#include "Analysis/AnalysisMethod.h"

class AnalysisController : public QThread {
    Q_OBJECT
public:
    AnalysisController(std::string file_path, ANALYSIS_TYPE type, QObject* parent = 0);
    void run() override;
private:
    AnalysisMethod* method;
signals:
    void save_analysis(Analysis analysis);

private slots:
    void on_start();                // Start or resume the analysis
    void on_pause();                // Pause the analysis
    void on_abort();                // Abort the analysis
};

#endif // ANALYSISCONTROLLER_H
