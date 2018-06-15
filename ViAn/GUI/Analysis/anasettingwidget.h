#ifndef ANASETTINGWIDGET_H
#define ANASETTINGWIDGET_H
#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include "Analysis/analysissettings.h"

class AnaSettingWidget : public QWidget
{
    Q_OBJECT
public:
    AnaSettingWidget(AnalysisSettings *settings, QWidget* parent = 0);
    AnalysisSettings* m_settings = nullptr;
    
    QFormLayout* form_layout;
    std::map<std::string, QLabel*> settings_map;


    void init_settings();
public slots:
    void set_ana_settings(AnalysisSettings *settings);

};

#endif // ANASETTINGWIDGET_H
