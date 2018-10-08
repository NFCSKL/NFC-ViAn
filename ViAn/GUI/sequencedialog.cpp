#include "sequencedialog.h"

SequenceDialog::SequenceDialog(QString* name, int* type, QWidget* parent) : QDialog(parent) {
    setWindowTitle(tr("Vian - Import image sequence"));
    setAttribute(Qt::WA_DeleteOnClose);

    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    m_name = name;
    m_type = type;
    
    // Setup widget layout
    QVBoxLayout* vertical_layout = new QVBoxLayout;
    name_text = new QLineEdit(this);
    name_text->setText("Sequence");
    video_type = new QRadioButton;
    video_type->setText("Video");
    video_type->setChecked(true);
    tag_type = new QRadioButton;
    tag_type->setText("Tag");

    name_text->setText("Sequence");
    btn_box = new QDialogButtonBox(Qt::Horizontal);
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Cancel);

    QFormLayout* sequence_name_layout = new QFormLayout;
    sequence_name_layout->addRow("Sequence name:", name_text);

    QVBoxLayout* type_btns = new QVBoxLayout();
    type_btns->addWidget(video_type);
    type_btns->addWidget(tag_type);

    QFormLayout* sequence_type_layout = new QFormLayout;
    sequence_type_layout->addRow("Open images as: ", type_btns);

    vertical_layout->addLayout(sequence_name_layout);
    vertical_layout->addLayout(sequence_type_layout);
    vertical_layout->addWidget(btn_box);

    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &SequenceDialog::ok_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &SequenceDialog::cancel_btn_clicked);
    setLayout(vertical_layout);

}

SequenceDialog::~SequenceDialog() {
    delete m_name;
    delete name_text;
}

void SequenceDialog::ok_btn_clicked() {
    *m_name = name_text->text();
    if (video_type->isChecked()) {
        *m_type = 0;
    } else if (tag_type->isChecked()) {
        *m_type = 1;
    }
    accept();
}

void SequenceDialog::cancel_btn_clicked() {
    reject();
}
