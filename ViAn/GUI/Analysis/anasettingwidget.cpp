#include "anasettingwidget.h"

AnaSettingWidget::AnaSettingWidget(AnalysisSettings* settings, QWidget *parent) : QWidget(parent) {
    setWindowTitle("Vian - Analysis settings");
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
    std::vector<std::string> vars = m_settings->get_var_names();
    for (std::string name : vars) {
        settings_map[name]->setText(QString::number(m_settings->get_setting(name)));
    }
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
}
