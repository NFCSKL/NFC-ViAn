#ifndef ANALYSISCONTROLLER_H
#define ANALYSISCONTROLLER_H

#include <QThread>
#include "Filehandler/analysis.h"
#include "Analysis/AnalysisMethod.h"

class AnalysisController : public QThread {
    Q_OBJECT
public:
    AnalysisController(std::string file_path, ANALYSIS_TYPE type, QObject* parent = 0);
    AnalysisController(std::string file_path, ANALYSIS_TYPE type, std::vector<cv::Point> inclusion_exclusion_points, bool exclude_poly, QObject* parent = 0);
    void run() override;
private:
    void setup_analysis(std::string file_path, ANALYSIS_TYPE type);
    AnalysisMethod* method;
signals:
    void save_analysis(Analysis analysis);
    void show_analysis_progress(int progress);

private slots:
    void on_start();                // Start or resume the analysis
    void on_pause();                // Pause the analysis
    void on_abort();                // Abort the analysis
    void on_progress_update(int progress);
};

#endif // ANALYSISCONTROLLER_H
