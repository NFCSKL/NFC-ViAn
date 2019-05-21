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
    if (settings->type != m_settings->type) {
        // Add both ana settings to the widget or somehow change between them
        // And fix frame num in yololist widget







    }
//    qDebug() << "type" << settings->type;
//    settings_map.clear();
    m_settings = settings;
//    form_layout = new QFormLayout();
//    init_settings();
    std::vector<std::string> vars = m_settings->get_var_names();
    for (std::string name : vars) {
        settings_map[name]->setText(QString::number(m_settings->get_setting(name)));
    }
    settings_map["Interval"]->setText(QString::number(m_settings->interval.first) + " - " + QString::number(m_settings->interval.second));
    settings_map["Bounding box pos (X,Y)"]->setText(QString::number(m_settings->bounding_box.x) + ", " + QString::number(m_settings->bounding_box.y));
    settings_map["Bounding box size (w, h)"]->setText(QString::number(m_settings->bounding_box.width) + ", " + QString::number(m_settings->bounding_box.height));
    repaint();
}

/**
 * @brief AnaSettingWidget::init_settings
 * Initialize the fields with the settings and save them to a map for easy editing
 */
void AnaSettingWidget::init_settings() {
    std::vector<std::string> vars = m_settings->get_var_names();
    for (std::string name : vars) {
        QLabel* value = new QLabel(QString::number(m_settings->get_setting(name)));
        form_layout->addRow(QString::fromStdString(name), value);
        settings_map.insert(std::make_pair(name, value));
    }
    QLabel* interval_label = new QLabel("0");
    QLabel* b_box_xy_label = new QLabel("0");
    QLabel* b_box_size_label = new QLabel("0");
    form_layout->addRow("Interval", interval_label);
    form_layout->addRow("Bounding box pos (X,Y)", b_box_xy_label);
    form_layout->addRow("Bounding box size (w, h)", b_box_size_label);
    settings_map.insert(std::make_pair("Interval", interval_label));
    settings_map.insert(std::make_pair("Bounding box pos (X,Y)", b_box_xy_label));
    settings_map.insert(std::make_pair("Bounding box size (w, h)", b_box_size_label));
}
