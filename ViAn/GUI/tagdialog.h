#ifndef TAGDIALOG_H
#define TAGDIALOG_H
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>

class TagDialog : public QDialog
{
    Q_OBJECT
    QLineEdit* name;
    QDialogButtonBox* btn_box;
public:
    explicit TagDialog(QWidget *parent = nullptr);

signals:
    void tag_name(QString);

private slots:
    void ok_btn_clicked();
    void cancel_btn_clicked();
};

#endif // TAGDIALOG_H
