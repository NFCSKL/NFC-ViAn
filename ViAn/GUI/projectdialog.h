#ifndef PROJECTDIALOG_H
#define PROJECTDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QString>
#include <QStandardPaths>

class ProjectDialog : public QDialog
{
    Q_OBJECT
    QString* m_name;
    QString* m_path;
    QLineEdit* path_text;
    QLineEdit* name_text;
    QDialogButtonBox* btn_box;
    const QString default_path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ViAn Projects";
public:
    explicit ProjectDialog(QString* name, QString* path, QWidget *parent = nullptr);

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
