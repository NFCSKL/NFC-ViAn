#ifndef MANIPULATORWIDGET_H
#define MANIPULATORWIDGET_H

#include <Video/framemanipulator.h>

#include <QWidget>

class QDialogButtonBox;
class QFormLayout;
class QLabel;
class QPushButton;
class QSlider;

class ManipulatorWidget : public QWidget {
    Q_OBJECT
    int brightness;
    double contrast;
    double gamma;

    QFormLayout* layout;
    QFormLayout* brightness_layout;
    QFormLayout* contrast_layout;
    QFormLayout* gamma_layout;
    QLabel* brightness_value_label;
    QLabel* contrast_value_label;
    QLabel* gamma_value_label;
    QSlider* brightness_slider;
    QSlider* contrast_slider;
    QSlider* gamma_slider;
    QDialogButtonBox* btn_box;
    QPushButton* apply_btn;

signals:
    void values(int b_value, double c_value, double g_value, bool update);
    void update_tag(int b_value, double c_value, double g_value);

public:
    ManipulatorWidget(int b = FrameManipulator().BRIGHTNESS_DEFAULT,
                      double c = FrameManipulator().CONTRAST_DEFAULT,
                      double g = FrameManipulator().GAMMA_DEFAULT,
                      QWidget* parent = nullptr);

    QFormLayout* form_layout;

private slots:
    void ok_clicked();
    void default_clicked();
    void b_changed(int value);
    void c_changed(int value);
    void g_changed(int value);

public slots:
    void set_values(int b_value, double c_value, double g_value);
};

#endif // MANIPULATORWIDGET_H
