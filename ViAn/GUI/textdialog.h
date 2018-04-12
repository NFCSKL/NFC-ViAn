#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QDoubleSpinBox>

#include "Video/shapes/text.h"

class TextDialog : public QDialog
{
    Q_OBJECT
    QLineEdit* name;
    QDialogButtonBox* btn_box;
    QDoubleSpinBox* double_box;
    float current_font_scale = 1;
public:
    TextDialog(QWidget *parent = nullptr);
signals:
    void text(QString, float);

private slots:
    void ok_btn_clicked();
    void cancel_btn_clicked();
};

#endif // TEXTDIALOG_H
