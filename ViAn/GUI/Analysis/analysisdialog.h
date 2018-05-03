#ifndef ANALYSISDIALOG_H
#define ANALYSISDIALOG_H
#include <QDialog>
#include <QLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QLineEdit>
#include <QAbstractItemView>
#include "GUI/TreeItems/videoitem.h"
#include "GUI/TreeItems/analysisitem.h"
#include "videolistitem.h"
class VideoItem;
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
    // Directory to save analysis
    std::string m_save_dir;

    MotionDetection* method;
public:
    AnalysisDialog(std::vector<VideoItem*> vid_projs, string save_dir);
    ~AnalysisDialog();
public slots:
    void ok_btn_clicked();
    void cancel_btn_clicked();
    void item_changed();
signals:
    // Send to start analysis
    void start_analysis(AnalysisMethod* method, VideoProject* vid_proj);
private:
    // Add settings to form layout
    void add_settings(QFormLayout* form);
    // Set setting sn form layout to method
    void set_settings(AnalysisMethod* method);
};

#endif // ANALYSISDIALOG_H
