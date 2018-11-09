#ifndef TAGDIALOG_H
#define TAGDIALOG_H

#include <QDialog>

class QDialogButtonBox;
class QLineEdit;

/**
 * @brief The TagDialog class
 * This dialog is used to create new
 * tags
 */
class TagDialog : public QDialog
{
    Q_OBJECT
    // name of tag
    QLineEdit* name;
    // ok,cancel btns
    QDialogButtonBox* btn_box;
public:
    explicit TagDialog(QWidget *parent = nullptr);
    ~TagDialog();
signals:
    // Send tag name
    void tag_name(QString);

private slots:
    void ok_btn_clicked();
    void cancel_btn_clicked();
};

#endif // TAGDIALOG_H
