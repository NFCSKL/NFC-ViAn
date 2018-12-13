#ifndef PATHDIALOG_H
#define PATHDIALOG_H

#include <QDialog>
#include <QDir>

class QDialogButtonBox;
class QLineEdit;
class PathDialog : public QDialog
{
    Q_OBJECT
    QString* m_path;

    QString m_q_path;
    QString m_dir;
    QString m_name;
    QLineEdit* path_text;
    QDialogButtonBox* btn_box;
    QString m_default_path;

    void enable_ok_btn(const bool& enable);
public:
    explicit PathDialog(QString *path, QWidget* parent = nullptr, QString default_path = QDir::homePath());

private slots:
    void ok_btn_clicked();
    void browse_btn_clicked();
    void path_btn_clicked();
    void cancel_btn_clicked();
signals:
    void open_view_path_dialog();
};

#endif // PATHDIALOG_H
