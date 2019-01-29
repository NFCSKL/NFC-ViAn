#ifndef SEQUENCEDIALOG_H
#define SEQUENCEDIALOG_H

#include <QDialog>


class QDialogButtonBox;
class QLineEdit;
class QRadioButton;
class SequenceDialog : public QDialog
{
    Q_OBJECT
    QString* m_name;        // Name of sequence
    int* m_type;           // Type of sequence, 0 = video, 1 = tag
    QLineEdit* name_text;
    QDialogButtonBox* btn_box;

    QRadioButton* video_type;
    QRadioButton* tag_type;
public:
    explicit SequenceDialog(QString* name, int* type, QWidget* parent = nullptr);
    ~SequenceDialog();

 private slots:
    void ok_btn_clicked();
    void cancel_btn_clicked();
};

#endif // SEQUENCEDIALOG_H
