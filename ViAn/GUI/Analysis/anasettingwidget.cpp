#include "anasettingwidget.h"

#include "Analysis/analysissettings.h"

#include <QFormLayout>
#include <QLabel>

#include <QDebug>

AnaSettingWidget::AnaSettingWidget(AnalysisSettings* settings, QWidget *parent) : QWidget(parent) {
    setWindowTitle("Vian - Analysis info");
    form_layout = new QFormLayout();
    setLayout(form_layout);
    m_settings = settings;
    init_settings();
    show();
}

/**
 * @brief AnaSettingWidget::set_ana_settings
 * Set a new AnalysisSettings and update the widget with the new values.
 * @param settings
 */
void AnaSettingWidget::set_ana_settings(AnalysisSettings* settings) {
    m_settings = settings;
    // Update motion detection variables
    std::vector<std::string> motion_vars = m_settings->get_motion_var_names();
    for (std::string name : motion_vars) {
        settings_map_motion[name]->setText(QString::number(m_settings->get_motion_setting(name)));
        if (m_settings->type == MOTION_DETECTION) {
            settings_map_motion[name]->show();
        } else {
            settings_map_motion[name]->hide();
        }
    }
    // Update object detection variables
    std::vector<std::string> object_vars = m_settings->get_object_var_names();
    for (std::string name : object_vars) {
        settings_map_object[name]->setText(QString::number(m_settings->get_object_setting(name)));
        if (m_settings->type == OBJECT_DETECTION) {
            settings_map_object[name]->show();
        } else {
            settings_map_object[name]->hide();
        }
    }

    settings_map_default["Interval"]->setText(QString::number(m_settings->interval.first) + " - " + QString::number(m_settings->interval.second));
    settings_map_default["Bounding box pos (X,Y)"]->setText(QString::number(m_settings->bounding_box.x) + ", " + QString::number(m_settings->bounding_box.y));
    settings_map_default["Bounding box size (w, h)"]->setText(QString::number(m_settings->bounding_box.width) + ", " + QString::number(m_settings->bounding_box.height));
    repaint();
}

/**
 * @brief AnaSettingWidget::init_settings
 * Initialize the fields with the settings and save them to a map for easy editing
 */
void AnaSettingWidget::init_settings() {
    // Add motion detection variables
    std::vector<std::string> motion_vars = m_settings->get_motion_var_names();
    for (std::string name : motion_vars) {
        QLabel* value = new QLabel(QString::number(m_settings->get_motion_setting(name)));
        form_layout->addRow(QString::fromStdString(name), value);
        settings_map_motion.insert(std::make_pair(name, value));
        if (m_settings->type != MOTION_DETECTION) value->hide();
    }
    // Add object detection variables
    std::vector<std::string> object_vars = m_settings->get_object_var_names();
    for (std::string name : object_vars) {
        QLabel* value = new QLabel(QString::number(m_settings->get_object_setting(name)));
        form_layout->addRow(QString::fromStdString(name), value);
        settings_map_object.insert(std::make_pair(name, value));
        if (m_settings->type != OBJECT_DETECTION) value->hide();
    }

    QLabel* interval_label = new QLabel("0");
    QLabel* b_box_xy_label = new QLabel("0");
    QLabel* b_box_size_label = new QLabel("0");
    form_layout->addRow("Interval", interval_label);
    form_layout->addRow("Bounding box pos (X,Y)", b_box_xy_label);
    form_layout->addRow("Bounding box size (w, h)", b_box_size_label);
    settings_map_default.insert(std::make_pair("Interval", interval_label));
    settings_map_default.insert(std::make_pair("Bounding box pos (X,Y)", b_box_xy_label));
    settings_map_default.insert(std::make_pair("Bounding box size (w, h)", b_box_size_label));
}
