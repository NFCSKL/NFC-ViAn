#ifndef MANIPULATORDIALOG_H
#define MANIPULATORDIALOG_H

#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QDialogButtonBox>

/**
 * @brief The ManipulatorDialog class
 * Dialog used to modify brightness and contrast.
 */
class ManipulatorDialog : public QDialog {
    Q_OBJECT
    const double DOUBLE_TO_INT = 100;
    double contrast;
    int brightness;

    QFormLayout* layout;
    QFormLayout* brightness_layout;
    QFormLayout* contrast_layout;
    QLabel* brightness_value_label;
    QLabel* contrast_value_label;
    QSlider* brightness_slider;
    QSlider* contrast_slider;
    QSpinBox* brightness_box;
    QDoubleSpinBox* contrast_box;
    QDialogButtonBox* btn_box;
signals:
    void values(int b_value, double c_value);
public:
    explicit ManipulatorDialog(int b, double c, QWidget *parent = nullptr);
private slots:
    void ok_clicked();
    void cancel_clicked();
    void reset_clicked();
    void b_changed(int value);
    void c_changed(int value);
};

#endif // MANIPULATORDIALOG_H
