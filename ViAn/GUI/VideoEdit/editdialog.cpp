#include "editdialog.h"

#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

#include <QDebug>

EditDialog::EditDialog(int* start, int* end, int max, QWidget *parent) : QDialog(parent) {
    setWindowTitle("ViAn - Edit interval");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setFocusPolicy(Qt::ClickFocus);
    setMinimumSize(230, 100);

    m_start = start;
    m_end = end;
    m_max = max;

    m_from_box = new QSpinBox(this);
    m_to_box = new QSpinBox(this);

    m_from_box->setRange(0, max);
    m_to_box->setRange(0, max);

    m_from_box->setValue(*start);
    m_to_box->setValue(*end);

    m_from_box->setKeyboardTracking(false);
    m_to_box->setKeyboardTracking(false);

    connect(m_from_box, QOverload<int>::of(&QSpinBox::valueChanged), this, &EditDialog::from_value_changed);
    connect(m_to_box, QOverload<int>::of(&QSpinBox::valueChanged), this, &EditDialog::to_value_changed);

    // Add input fields to layout
    m_input_layout = new QHBoxLayout();
    m_input_layout->addWidget(new QLabel(tr("From:")));
    m_input_layout->addWidget(m_from_box);
    m_input_layout->addWidget(new QLabel(tr("To:")));
    m_input_layout->addWidget(m_to_box);

    // Add buttons
    m_btn_box = new QDialogButtonBox();
    m_btn_box->addButton(QDialogButtonBox::Ok);
    m_btn_box->addButton(QDialogButtonBox::Cancel);
    connect(m_btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &EditDialog::ok_btn_clicked);
    connect(m_btn_box->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &EditDialog::cancel_btn_clicked);

    QVBoxLayout* m_layout = new QVBoxLayout(this);
    m_layout->addLayout(m_input_layout);
    m_layout->addWidget(m_btn_box);

    setLayout(m_layout);


}

void EditDialog::from_value_changed(int new_value) {
    if (new_value > m_to_box->value()) {
        m_from_box->setValue(m_to_box->value());
    }
}

void EditDialog::to_value_changed(int new_value) {
    if (new_value < m_from_box->value()) {
        m_to_box->setValue(m_from_box->value());
    }
}

void EditDialog::ok_btn_clicked() {
    *m_start = m_from_box->value();
    *m_end = m_to_box->value();

    accept();
}

void EditDialog::cancel_btn_clicked() {
    reject();
}
