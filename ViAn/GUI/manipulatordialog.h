#ifndef MANIPULATORDIALOG_H
#define MANIPULATORDIALOG_H

#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QDialogButtonBox>

class ManipulatorDialog : public QDialog {
    Q_OBJECT
    QSpinBox* brightness_box;
    QDoubleSpinBox* contrast_box;
    QDialogButtonBox* btn_box;
signals:
    void values(int b_value, double c_value);
public:
    explicit ManipulatorDialog(QWidget *parent = nullptr);
private slots:
    void ok_clicked();
    void apply_clicked();
    void cancel_clicked();
    void b_changed(int value);
    void c_changed(double value);
};

#endif // MANIPULATORDIALOG_H
