#ifndef GENERATEVIDEODIALOG_H
#define GENERATEVIDEODIALOG_H

#include <QDialog>

class QCheckBox;
class QComboBox;
class QDialogButtonBox;
class QLineEdit;
class GenerateVideoDialog : public QDialog
{
    Q_OBJECT
    QLineEdit* name_edit;
    QDialogButtonBox* btn_box;
    QComboBox* resolution;
    QCheckBox* keep_size_box;
    QCheckBox* custom_size_box;
    QLineEdit* custom_width;
    QLineEdit* custom_height;
    QComboBox* frame_rate;
public:
    explicit GenerateVideoDialog(std::vector<QSize> sizes, std::vector<int> fpses, QWidget* parent = nullptr);
    ~GenerateVideoDialog();

    void get_values(QString* name, QSize* size, int* fps, bool *keep_size);

private slots:
    void keep_size_toggled(int state);
    void custom_size_toggled(int state);
    void ok_btn_clicked();
    void cancel_btn_clicked();

};

#endif // GENERATEVIDEODIALOG_H
