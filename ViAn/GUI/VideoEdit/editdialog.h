#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>

class QDialogButtonBox;
class QHBoxLayout;
class QSpinBox;
class EditDialog : public QDialog
{
    int* m_start;
    int* m_end;
    int m_max;
    QHBoxLayout* m_input_layout;
    QSpinBox* m_from_box;
    QSpinBox* m_to_box;
    QDialogButtonBox* m_btn_box;
public:
    EditDialog(int *start, int *end, int max, QWidget* parent = nullptr);

private slots:

    void from_value_changed(int new_value);
    void to_value_changed(int new_value);
    void ok_btn_clicked();
    void cancel_btn_clicked();
};

#endif // EDITDIALOG_H
