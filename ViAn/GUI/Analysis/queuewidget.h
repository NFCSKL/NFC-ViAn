#ifndef QUEUEWIDGET_H
#define QUEUEWIDGET_H

#include <QWidget>

class AnalysisMethod;
class QListWidget;
class QLabel;
class QProgressBar;
class QPushButton;

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
    QueueWidget(QWidget* parent = nullptr);
    // Move queue forward
    void next();
    // Append method to queue
    void enqueue(AnalysisMethod *method);
    // Update progressbar
    void update_progress(int i);

signals:
    // Analysis aborted
    void abort_analysis();
};

#endif // QUEUEWIDGET_H
