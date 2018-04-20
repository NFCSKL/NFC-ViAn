#ifndef PROJECTDIALOG_H
#define PROJECTDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QString>

class ProjectDialog : public QDialog
{
    Q_OBJECT
    QLineEdit* path_text;
    QLineEdit* name_text;
    QDialogButtonBox* btn_box;
public:
    explicit ProjectDialog(QWidget *parent = nullptr, QString name = "");

signals:
    void project_path(QString name, QString path);
    void open_project(QString proj_path);

public slots:

private slots:
    void browse_btn_clicked(void);
    void ok_btn_clicked(void);
    void cancel_btn_clicked(void);
};

#endif // PROJECTDIALOG_H
