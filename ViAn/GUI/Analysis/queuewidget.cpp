#include "queuewidget.h"


QueueWidget::QueueWidget(QWidget *parent) : QWidget(parent) {
    m_queue = new QListWidget();
    setWindowTitle(QString::fromStdString("Vian - AnalysisQueue"));
//    m_queue->setAcceptDrops(true);
//    m_queue->setDragEnabled(true);

//    m_queue->setSelectionMode(QAbstractItemView::SingleSelection);
//    m_queue->setDropIndicatorShown(true);
//    m_queue->setDragDropMode(QAbstractItemView::InternalMove);
    abort_btn = new QPushButton(QIcon("../ViAn/Icons/abort.png"),"",this);
    QVBoxLayout* layout = new QVBoxLayout();
    QHBoxLayout* progress_layout = new QHBoxLayout();
    m_line = new QLabel();
    progressbar = new QProgressBar();
    progress_layout->addWidget(m_line);
    progress_layout->addWidget(progressbar);
    progress_layout->addWidget(abort_btn);
    connect(abort_btn, &QPushButton::pressed, this, &QueueWidget::abort_analysis);
    layout->addLayout(progress_layout);
    layout->addWidget(m_queue);
    setLayout(layout);
    show();
}

void QueueWidget::next()
{
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

void QueueWidget::enqueue(AnalysisMethod *method)
{
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

void QueueWidget::update_progress(int i)
{    
    progressbar->setValue(i);
}

void QueueWidget::toggle_show()
{
    if(this->isVisible()) hide();
    else show();
}

