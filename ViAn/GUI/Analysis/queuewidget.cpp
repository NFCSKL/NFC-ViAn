#include "queuewidget.h"
#include <QPushButton>

/**
 * @brief QueueWidget::QueueWidget
 * @param parent
 * Setup widget layout
 */
QueueWidget::QueueWidget(QWidget *parent) : QWidget(parent) {
    // Set Title
    setWindowTitle(QString::fromStdString("Vian - Analysis queue"));
    // Create queue
    m_queue = new QListWidget();
    // Create Abort button
    abort_btn = new QPushButton(QIcon("../ViAn/Icons/abort.png"),"",this);
    // Create main layout
    QVBoxLayout* layout = new QVBoxLayout();
    // Layout for current analysis
    QHBoxLayout* progress_layout = new QHBoxLayout();
    m_line = new QLabel();
    progressbar = new QProgressBar();
    progress_layout->addWidget(m_line);
    progress_layout->addWidget(progressbar);
    progress_layout->addWidget(abort_btn);
    // Connect abort button to abort
    connect(abort_btn, &QPushButton::pressed, this, &QueueWidget::abort_analysis);
    // Add current analysis layout to main layout
    layout->addLayout(progress_layout);
    // Add queue
    layout->addWidget(m_queue);    
    setLayout(layout);
    if(m_queue->count() == 0){
        abort_btn->hide();
    }
    // Show widget
    show();
}

/**
 * @brief QueueWidget::next
 * Move queue forward if queue non empty,
 * otherwise reset current analysis representation
 */
void QueueWidget::next() {
    QListWidgetItem* item;
    m_line->setText(QString(""));
    progressbar->setValue(0);
    if(m_queue->count() != 0){
        item = m_queue->takeItem(0);
        m_line->setText(item->text());
    }else{
        abort_btn->hide();
        progressbar->hide();
    }
}

/**
 * @brief QueueWidget::enqueue
 * @param method
 * Add method to back of queue
 */
void QueueWidget::enqueue(AnalysisMethod *method) {
    abort_btn->show();
    progressbar->show();
    AnalysisListItem* item = new AnalysisListItem(method);
    std::string path = method->save_path();
    std::size_t index = path.find_last_of('/') + 1;
    std::string analysis_name = path.substr(index);
    index = analysis_name.find_last_of('.');
    analysis_name = analysis_name.substr(0,index);

    item->setText(QString::fromStdString(analysis_name));
    m_queue->addItem(item);
}

/**
 * @brief QueueWidget::update_progress
 * @param i
 * Update progressbar progress
 */
void QueueWidget::update_progress(int i) {
    progressbar->setValue(i);
}

