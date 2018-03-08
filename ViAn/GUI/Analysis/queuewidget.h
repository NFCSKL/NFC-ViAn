#ifndef QUEUEWIDGET_H
#define QUEUEWIDGET_H
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLayout>
#include <QProgressBar>
#include "videolistitem.h"

/**
 * @brief The QueueWidget class
 * Widget for queue of analyses
 */
class QueueWidget : public QWidget
{
    Q_OBJECT
    // List containing queue
    QListWidget* m_queue;

    // Label containing name of current analysis
    QLabel* m_line;

    // Analysis progressbar
    QProgressBar* progressbar;

    // Button to abort analysis
    QPushButton* abort_btn;

public:
    QueueWidget(QWidget* parent = 0);
    // Move queue forward
    void next();
    // Append method to queue
    void enqueue(AnalysisMethod *method);
    // Update progressbar
    void update_progress(int i);
public slots:

signals:
    // Analysis aborted
    void abort_analysis();
};

#endif // QUEUEWIDGET_H
