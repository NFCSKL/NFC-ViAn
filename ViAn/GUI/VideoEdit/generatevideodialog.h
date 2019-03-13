#ifndef GENERATEVIDEODIALOG_H
#define GENERATEVIDEODIALOG_H

#include <QDialog>

class QComboBox;
class QDialogButtonBox;
class QLineEdit;
class GenerateVideoDialog : public QDialog
{
    Q_OBJECT
    QLineEdit* name_edit;
    QDialogButtonBox* btn_box;
    QComboBox* resolution;
    QComboBox* frame_rate;
public:
    explicit GenerateVideoDialog(std::vector<QSize> sizes, std::vector<int> fpses, QWidget* parent = nullptr);
    ~GenerateVideoDialog();

    void get_values(QString* name, QSize* size, int* fps);

private slots:
    void ok_btn_clicked();
    void cancel_btn_clicked();

};

#endif // GENERATEVIDEODIALOG_H
