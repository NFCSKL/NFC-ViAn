#ifndef ANASETTINGWIDGET_H
#define ANASETTINGWIDGET_H

#include <QWidget>

class AnalysisSettings;
class QFormLayout;
class QLabel;
class AnaSettingWidget : public QWidget
{
    Q_OBJECT
public:
    AnaSettingWidget(AnalysisSettings *settings, QWidget* parent = nullptr);
    AnalysisSettings* m_settings = nullptr;
    
    QFormLayout* form_layout;
    std::map<std::string, QLabel*> settings_map_default;
    std::map<std::string, QLabel*> settings_map_motion;
    std::map<std::string, QLabel*> settings_map_object;


    void init_settings();
public slots:
    void set_ana_settings(AnalysisSettings *settings);

};

#endif // ANASETTINGWIDGET_H
