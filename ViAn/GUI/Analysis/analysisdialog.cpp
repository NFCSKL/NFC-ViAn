#include "analysisdialog.h"

#include "Analysis/analysissettings.h"
#include "GUI/TreeItems/videoitem.h"
#include "Project/videoproject.h"
#include "videolistitem.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFile>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

AnalysisDialog::AnalysisDialog(std::vector<VideoItem *> vid_projs, AnalysisSettings *analysis_settings)
    : m_analysis_settings(analysis_settings) {
    // Set title
    setWindowTitle("Vian - Edit analysis settings");
    // Remove "?" button
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setAttribute(Qt::WA_DeleteOnClose);

    // Box layout to store video projects
    QVBoxLayout* v_lay = new QVBoxLayout();
    if (vid_projs.empty()) do_analysis = false;


    if (do_analysis) {
        setWindowTitle("Vian - Full analysis");

        QLabel* select_files = new QLabel();
        select_files->setText("Select files to analyze");
        v_lay->addWidget(select_files);
        m_v_proj_list = new QListWidget(this);

        // Windows-like file selection
        for(VideoItem* v : vid_projs) {
            QString path = v->get_video_project()->get_video()->file_path;
            QFile load_file(path);
            if (load_file.open(QIODevice::ReadOnly)) {
                VideoListItem* item = new VideoListItem(v->get_video_project());
                item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                item->setCheckState(Qt::Unchecked);
                m_v_proj_list->addItem(item);
                if (v->get_video_project()->is_current()) {
                    item->setCheckState(Qt::Checked);
                    m_v_proj_list->setCurrentItem(item);
                }
            }
        }
        v_lay->addWidget(m_v_proj_list);

        connect(m_v_proj_list, &QListWidget::itemChanged, this, &AnalysisDialog::item_changed);
    }

    QLabel* settings_label = new QLabel();
    settings_label->setText("Settings");
    v_lay->addWidget(settings_label);

    // Add analysis form
    QFormLayout* form_lay = new QFormLayout();
    v_lay->addLayout(form_lay);
    add_settings(form_lay);

    // Add Buttons
    btn_box = new QDialogButtonBox;
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Cancel);
    btn_box->addButton(QDialogButtonBox::Reset);
    if (do_analysis && m_v_proj_list->selectedItems().size() <= 0) btn_box->button(QDialogButtonBox::Ok)->setDisabled(true);

    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &AnalysisDialog::ok_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &AnalysisDialog::cancel_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Reset), &QPushButton::clicked, this, &AnalysisDialog::reset_settings);

    v_lay->addWidget(btn_box);
    setLayout(v_lay);
}

AnalysisDialog::~AnalysisDialog() {

}

/**
 * @brief AnalysisDialog::ok_btn_clicked
 * Okay clicked, if any selected videos, execute analyses for them
 */
void AnalysisDialog::ok_btn_clicked() {
    if (do_analysis) {
        for (int i = 0; i < m_v_proj_list->count(); i++) {
            if (m_v_proj_list->item(i)->checkState() == Qt::Checked) {
                VideoProject* vid_proj = dynamic_cast<VideoListItem*>(m_v_proj_list->item(i))->m_vid_proj;
                set_settings();
                emit start_analysis(vid_proj, m_analysis_settings);
                close();
            }
        }
        return;
    }
    set_settings();
    close();
}

/**
 * @brief AnalysisDialog::cancel_btn_clicked
 * Cancel clicked, close window
 */
void AnalysisDialog::cancel_btn_clicked() {
    close();
}

void AnalysisDialog::item_changed() {
    qDebug() << "item changed";
    for (int i = 0; i < m_v_proj_list->count(); i++) {
        if (m_v_proj_list->item(i)->checkState() == Qt::Checked) {
            btn_box->button(QDialogButtonBox::Ok)->setEnabled(true);
            return;
        }
    }
    btn_box->button(QDialogButtonBox::Ok)->setDisabled(true);
}

/**
 * @brief AnalysisDialog::add_settings
 * @param form
 * Add Settings to form layout
 */
void AnalysisDialog::add_settings(QFormLayout *form) {
    std::vector<std::string> vars;
    if (m_analysis_settings->type == MOTION_DETECTION) {
        vars = m_analysis_settings->get_motion_var_names();
    } else if (m_analysis_settings->type == OBJECT_DETECTION) {
        vars = m_analysis_settings->get_object_var_names();
    }
    for(std::string name : vars) {
        QWidget* var = new QLineEdit("Error");
        if (m_analysis_settings->type == MOTION_DETECTION) {
            if (name == "OPEN_DEGREE") {
                QSpinBox* spin_var = new QSpinBox();
                spin_var->setMaximum(10);
                spin_var->setMinimum(0);
                spin_var->setValue(m_analysis_settings->get_motion_setting(name));
                var = spin_var;
            } else if (name == "SMALLEST_OBJECT_SIZE") {
                QSpinBox* spin_var = new QSpinBox();
                spin_var->setMaximum(1000);
                spin_var->setMinimum(10);
                spin_var->setValue(m_analysis_settings->get_motion_setting(name));
                var = spin_var;
            } else if (name == "BACKGROUND_HISTORY") {
                QSpinBox* spin_var = new QSpinBox();
                spin_var->setMaximum(1000);
                spin_var->setMinimum(10);
                spin_var->setValue(m_analysis_settings->get_motion_setting(name));
                var = spin_var;
            } else if (name == "MOG2_THRESHOLD") {
                QSpinBox* spin_var = new QSpinBox();
                spin_var->setMaximum(50);
                spin_var->setMinimum(0);
                spin_var->setValue(m_analysis_settings->get_motion_setting(name));
                var = spin_var;
            } else if (name == "DETECT_SHADOWS") {
                QCheckBox* check_var = new QCheckBox;
                var = check_var;
            } else if (name == "SAMPLE_FREQUENCY") {
                QSpinBox* spin_var = new QSpinBox();
                spin_var->setMaximum(1000);
                spin_var->setMinimum(1);
                spin_var->setValue(m_analysis_settings->get_motion_setting(name));
                var = spin_var;
            }
        } else if (m_analysis_settings->type == OBJECT_DETECTION) {
            if (name == "Network size") {
                QComboBox* combo_var = new QComboBox();
                combo_var->addItem(QString::number(608));
                combo_var->addItem(QString::number(416));
                combo_var->addItem(QString::number(320));
                combo_var->addItem(QString::number(256));
                combo_var->addItem(QString::number(128));
                combo_var->addItem(QString::number(64));
                combo_var->setCurrentText(QString::number(416));
                var = combo_var;
            } else if (name == "Sample frequency (frames/sec)") {
                QComboBox* combo_var = new QComboBox();
                combo_var->addItem(QString::number(100));
                combo_var->addItem(QString::number(10));
                combo_var->addItem(QString::number(5));
                combo_var->addItem(QString::number(2));
                combo_var->addItem(QString::number(1));
                combo_var->addItem(QString::number(0.5));
                combo_var->addItem(QString::number(0.2));
                combo_var->addItem(QString::number(0.1));
                combo_var->setCurrentText(QString::number(1));
                var = combo_var;
            } else if (name == "Confidence threshold") {
                QDoubleSpinBox* spin_var = new QDoubleSpinBox();
                spin_var->setMaximum(1);
                spin_var->setMinimum(0);
                spin_var->setSingleStep(0.1);
                spin_var->setValue(m_analysis_settings->get_object_setting(name));
                var = spin_var;
            } else if (name == "Nms threshold") {
                QDoubleSpinBox* spin_var = new QDoubleSpinBox();
                spin_var->setMaximum(1);
                spin_var->setMinimum(0);
                spin_var->setSingleStep(0.1);
                spin_var->setValue(m_analysis_settings->get_object_setting(name));
                var = spin_var;
            }
        } else {
            var = new QLineEdit();
        }
        var->setToolTip(QString::fromStdString(m_analysis_settings->get_descr(name)));
        form->addRow(QString::fromStdString(name), var);
        m_settings.insert(std::make_pair(name, var));
    }
}

/**
 * @brief AnalysisDialog::set_settings
 * @param method
 * Set form layout settings to method
 */
void AnalysisDialog::set_settings() {
    for(auto line : m_settings) {
        double val = -1;
        if (line.first == "OPEN_DEGREE") {
            QSpinBox* settings = dynamic_cast<QSpinBox*>(line.second);
            val = settings->value();
        } else if (line.first == "SMALLEST_OBJECT_SIZE") {
            QSpinBox* settings = dynamic_cast<QSpinBox*>(line.second);
            val = settings->value();
        } else if (line.first == "BACKGROUND_HISTORY") {
            QSpinBox* settings = dynamic_cast<QSpinBox*>(line.second);
            val = settings->value();
        } else if (line.first == "MOG2_THRESHOLD") {
            QSpinBox* settings = dynamic_cast<QSpinBox*>(line.second);
            val = settings->value();
        } else if (line.first == "DETECT_SHADOWS") {
            QCheckBox* settings = dynamic_cast<QCheckBox*>(line.second);
            val = settings->checkState();
        } else if (line.first == "SAMPLE_FREQUENCY") {
            QSpinBox* settings = dynamic_cast<QSpinBox*>(line.second);
            val = settings->value();

        } else if (line.first == "Network size") {
            QSpinBox* settings = dynamic_cast<QSpinBox*>(line.second);
            val = settings->text().toDouble();
            //val = settings->value();
        } else if (line.first == "Sample frequency (frames/sec)") {
            QSpinBox* settings = dynamic_cast<QSpinBox*>(line.second);
            val = settings->text().toDouble();
            //val = settings->value();
        } else if (line.first == "Confidence threshold") {
            QDoubleSpinBox* settings = dynamic_cast<QDoubleSpinBox*>(line.second);
            val = settings->value();
        } else if (line.first == "Nms threshold") {
            QDoubleSpinBox* settings = dynamic_cast<QDoubleSpinBox*>(line.second);
            val = settings->value();
        }
        m_analysis_settings->set_setting(line.first, val);
    }
}

void AnalysisDialog::reset_settings() {
    m_analysis_settings->reset_settings();
    for (auto line : m_settings) {
        if (line.first == "OPEN_DEGREE") {
            QSpinBox* settings = dynamic_cast<QSpinBox*>(line.second);
            settings->setValue(int(m_analysis_settings->get_motion_setting(line.first)));
        } else if (line.first == "SMALLEST_OBJECT_SIZE") {
            QSpinBox* settings = dynamic_cast<QSpinBox*>(line.second);
            settings->setValue(int(m_analysis_settings->get_motion_setting(line.first)));
        } else if (line.first == "BACKGROUND_HISTORY") {
            QSpinBox* settings = dynamic_cast<QSpinBox*>(line.second);
            settings->setValue(int(m_analysis_settings->get_motion_setting(line.first)));
        } else if (line.first == "MOG2_THRESHOLD") {
            QSpinBox* settings = dynamic_cast<QSpinBox*>(line.second);
            settings->setValue(int(m_analysis_settings->get_motion_setting(line.first)));
        } else if (line.first == "DETECT_SHADOWS") {
            QCheckBox* settings = dynamic_cast<QCheckBox*>(line.second);
            settings->setChecked(int(m_analysis_settings->get_motion_setting(line.first)));
        } else if (line.first == "SAMPLE_FREQUENCY") {
            QSpinBox* settings = dynamic_cast<QSpinBox*>(line.second);
            settings->setValue(int(m_analysis_settings->get_motion_setting(line.first)));

        } else if (line.first == "Network size") {
            QComboBox* settings = dynamic_cast<QComboBox*>(line.second);
            settings->setCurrentText(QString::number(m_analysis_settings->get_object_setting(line.first)));
        } else if (line.first == "Sample frequency (frames/sec)") {
            QComboBox* settings = dynamic_cast<QComboBox*>(line.second);
            settings->setCurrentText(QString::number(m_analysis_settings->get_object_setting(line.first)));
        } else if (line.first == "Confidence threshold") {
            QDoubleSpinBox* settings = dynamic_cast<QDoubleSpinBox*>(line.second);
            settings->setValue(double(m_analysis_settings->get_object_setting(line.first)));
        } else if (line.first == "Nms threshold") {
            QDoubleSpinBox* settings = dynamic_cast<QDoubleSpinBox*>(line.second);
            settings->setValue(double(m_analysis_settings->get_object_setting(line.first)));
        }

    }
}
