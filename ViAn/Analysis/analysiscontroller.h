#ifndef ANALYSISCONTROLLER_H
#define ANALYSISCONTROLLER_H

#include <QThread>
#include "Project/Analysis/analysisproxy.h"
#include "Analysis/analysismethod.h"
#include "analysissettings.h"
class AnalysisController : public QThread {
    Q_OBJECT

private:
    std::string m_save_path;
    std::string m_video_path;
public:
    AnalysisController(QObject* parent = 0);
    void run() override;
    void new_analysis(AnalysisMethod *method, std::string save_path);
private:
    void setup_analysis(AnalysisMethod *method);
    AnalysisMethod* method;
signals:
    void analysis_done(AnalysisProxy);
    void progress_signal(int);
private slots:
    void on_start();                // Start or resume the analysis
    void on_pause();                // Pause the analysis
    void on_abort();                // Abort the analysis
    void on_progress_update(int progress);
};

#endif // ANALYSISCONTROLLER_H
