#ifndef MANIPULATORWIDGET_H
#define MANIPULATORWIDGET_H

#include <QObject>
#include <QWidget>
#include <QFormLayout>
#include <QSlider>
#include <QLabel>
#include <QDialogButtonBox>
#include <Video/framemanipulator.h>

class ManipulatorWidget : public QWidget {
    Q_OBJECT
    const double DOUBLE_TO_INT = 100;
    int brightness;
    double contrast;

    QFormLayout* layout;
    QFormLayout* brightness_layout;
    QFormLayout* contrast_layout;
    QLabel* brightness_value_label;
    QLabel* contrast_value_label;
    QSlider* brightness_slider;
    QSlider* contrast_slider;
    QDialogButtonBox* btn_box;

signals:
    void values(int b_value, double c_value, bool update);
    void update_tag(int b_value, double c_value);

public:
    ManipulatorWidget(int b = FrameManipulator().BRIGHTNESS_DEFAULT, double c = FrameManipulator().CONTRAST_DEFAULT, QWidget* parent = 0);

    QFormLayout* form_layout;

private slots:
    void ok_clicked();
    void default_clicked();
    void b_changed(int value);
    void c_changed(int value);

public slots:
    void set_values(int b_value, double c_value);
};

#endif // MANIPULATORWIDGET_H
