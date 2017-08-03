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
class AnalysisDialog : public QDialog
{
    Q_OBJECT
    QListWidget* m_v_proj_list;
    std::map<std::string,QLineEdit*> m_settings;
    MotionDetection* method;
    std::string m_save_dir;

    QLineEdit* interval;
    QLineEdit* bounding_box;
public:
    AnalysisDialog(std::vector<VideoItem*> vid_projs, string save_dir);
public slots:
    void ok_btn_clicked();
    void cancel_btn_clicked();
signals:
    void start_analysis(AnalysisMethod* method, VideoProject* vid_proj);
private:
    void add_settings(QFormLayout* form);
    void set_settings(AnalysisMethod* method);
};

#endif // ANALYSISDIALOG_H
