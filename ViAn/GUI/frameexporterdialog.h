#ifndef FRAMEEXPORTERDIALOG_H
#define FRAMEEXPORTERDIALOG_H

#include <QBoxLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QFileDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QSpinBox>

#include "imageexporter.h"
#include "Project/video.h"

class FrameExporterDialog : public QDialog {
    Q_OBJECT
    QFormLayout* m_input_layout;
    QHBoxLayout* m_frame_input_layout;
    QHBoxLayout* m_path_input_layout;
    QLabel* m_video_path;
    QSpinBox* m_from_box;
    QSpinBox* m_to_box;
    QLabel* m_path_label;
    QLabel* m_total;
    QPushButton* m_browse_btn;
    QPushButton* m_export_btn;
    QDialogButtonBox* m_btn_box;

    // The ImageExporter should only be set from a constructor parameter
    // and as such deleted outside of this class
    ImageExporter* m_exporter = nullptr;
    QString m_video_name;
    QString m_old_path_text = "C:/";
public:
    FrameExporterDialog(ImageExporter* im_exp, Video* video, std::string proj_path,
                        const int& max_frame, const std::pair<int, int>& interval, QWidget* parent = nullptr);
private:
    void update_total();
    bool check_path();
private slots:
    void from_value_changed();
    void to_value_changed();
    void open_path_dialog();
    void save_values();
};

#endif // FRAMEEXPORTERDIALOG_H
