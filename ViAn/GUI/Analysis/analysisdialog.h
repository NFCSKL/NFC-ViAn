#ifndef ANALYSISDIALOG_H
#define ANALYSISDIALOG_H

#include <QDialog>


class AnalysisSettings;
class MotionDetection;
class QDialogButtonBox;
class QFormLayout;
class QListWidget;
class VideoItem;
class VideoProject;
/**
 * @brief The AnalysisDialog class
 *  This Dialog is used for advanced analysis
 */
class AnalysisDialog : public QDialog
{
    Q_OBJECT
    // Lists of videoprojects to potentially analyse
    QListWidget* m_v_proj_list;
    // Button box
    QDialogButtonBox* btn_box;
    // Variable settings fields
    std::map<std::string,QWidget*> m_settings;
    bool do_analysis = true;

    MotionDetection* method;
    AnalysisSettings* m_analysis_settings;
public:
    AnalysisDialog(std::vector<VideoItem*> vid_projs, AnalysisSettings* analysis_settings);
    ~AnalysisDialog();
public slots:
    void ok_btn_clicked();
    void cancel_btn_clicked();
    void item_changed();
signals:
    // Send to start analysis
    void start_analysis(VideoProject* vid_proj, AnalysisSettings* do_analysis);
private:
    // Add settings to form layout
    void add_settings(QFormLayout* form);
    // Set setting sn form layout to method
    void set_settings();
    // Reset the settings in default
    void reset_settings();
};

#endif // ANALYSISDIALOG_H
