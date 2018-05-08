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
    settings_map["Interval"]->setText(QString::number(m_settings->interval.first));
    settings_map["Bounding box"]->setText(QString::number(m_settings->interval.second));
    settings_map["Analysis name"]->setText("Heh");
    settings_map["Video name"]->setText("Yeah");
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
    QLabel* b_box_label = new QLabel("0");
    QLabel* name_label = new QLabel("Analysis");
    QLabel* video_label = new QLabel("Pumparna");
    form_layout->addRow(QString::fromStdString("Interval"), interval_label);
    form_layout->addRow(QString::fromStdString("Bounding box"), b_box_label);
    form_layout->addRow(QString::fromStdString("Analysis name"), name_label);
    form_layout->addRow(QString::fromStdString("Video name"), video_label);
    settings_map.insert(std::make_pair("Interval", interval_label));
    settings_map.insert(std::make_pair("Bounding box", b_box_label));
    settings_map.insert(std::make_pair("Analysis name", name_label));
    settings_map.insert(std::make_pair("Video name", video_label));

}
